#include <spdlog/spdlog.h>

#include <LangYa/Network/SerialPort.hpp>

#ifdef SC_WINDOWS
#define RESULT_MESSAGE(result) result.what()
#elif SC_LINUX
#define RESULT_MESSAGE(result) result.message()
#endif

namespace LangYa::Network
{
	using namespace Common;

	using IOContextType = boost::asio::io_context;
	using AsioSerialPort = boost::asio::serial_port;
	using ErrorCodeType = boost::system::error_code;

	SerialPort
	::SerialPort(IOContextType& ioContext, SerialPortInfo info) :
		UniqueSerialPort(std::make_unique<AsioSerialPort>(ioContext)),
		Info(std::move(info))
	{
	}

	void
	SerialPort
	::ApplyOption(boost::system::error_code& result) const
	{
		using AsioSerial = AsioSerialPort;

		UniqueSerialPort->set_option(
			AsioSerial::baud_rate(Info.BaudRate),
			result
		);
		if (result.failed()) return;

		UniqueSerialPort->set_option(
			AsioSerial::flow_control(AsioSerial::flow_control::none),
			result
		);
		if (result.failed()) return;

		UniqueSerialPort->set_option(
			AsioSerial::parity(AsioSerial::parity::none),
			result
		);
		if (result.failed()) return;

		UniqueSerialPort->set_option(
			AsioSerial::stop_bits(AsioSerial::stop_bits::one),
			result
		);
		if (result.failed()) return;

		UniqueSerialPort->set_option(
			AsioSerial::character_size(AsioSerial::character_size(Info.CharacterSize)),
			result
		);
	}

	void
	SerialPort
	::RefreshConnection(boost::system::error_code& result)
	{
		using namespace boost::asio;

		spdlog::info("SerialPort({})> Refreshing connection", Info.DeviceName);

		if (UniqueSerialPort->is_open())
		{
			spdlog::info("SerialPort({})> Is connected, doesn't need refresh.", Info.DeviceName);
			Info.IsConnected = true;
			result = boost::system::error_code();
			return;
		}

		Info.IsConnected = false;

		UniqueSerialPort->open(Info.DeviceName, result);
		if (result.failed())
		{
			spdlog::warn("SerialPort({})> Cannot open: {}", Info.DeviceName, RESULT_MESSAGE(result));
			return;
		}

		ApplyOption(result);
		if (result.failed())
		{
			spdlog::warn("SerialPort({})> Cannot set option: {}", Info.DeviceName, RESULT_MESSAGE(result));
			return;
		}

		Info.IsConnected = true;
	}

	std::shared_ptr<SerialPort>
	SerialPort::
	BuildShared(IOContextType& ioContext, const SerialPortInfo& info)
	{
		spdlog::info("Building Shared SerialPort({})", info.ToString());
		auto ptr = std::make_shared<SerialPort>(ioContext, info);
		spdlog::info("Finished building SerialPort({})", info.DeviceName);
		return ptr;
	}

	SerialPort
	::~SerialPort()
	{
		spdlog::info("SerialPort({})> Trying closing", Info.DeviceName);
		Close();
		spdlog::info("SerialPort({})> Closed", Info.DeviceName);
	}

	bool
	SerialPort
	::Open()
	{
		boost::system::error_code result;
		RefreshConnection(result);
		if (result.failed())
		{
			spdlog::warn("SerialPort({})> Cannot connect: {}", Info.DeviceName, RESULT_MESSAGE(result));
			return false;
		}
		return true;
	}

	void
	SerialPort
	::Close()
	{
		boost::system::error_code result;
		UniqueSerialPort->close(result);
		if (result.failed())
		{
			spdlog::warn("SerialPort({})> Cannot close serial port: {}", Info.DeviceName, RESULT_MESSAGE(result));
		}

		Info.IsConnected = false;
	}

	const
	SerialPortInfo&
	SerialPort::GetInfo() const
	{
		return Info;
	}

	MemoryView::SizeType
	SerialPort
	::Read(const MemoryView& view)
	{
		boost::system::error_code result{};
		auto bytes = read(*UniqueSerialPort, static_cast<boost::asio::mutable_buffer>(view), result);
		spdlog::info(
			"SerialPort({})> Read {}:{} bytes, failed({}), failure({})",
			Info.DeviceName,
			bytes,
			view.Size,
			result.failed(),
			RESULT_MESSAGE(result)
		);

		//TODO failure counter and exception processor

		return bytes;
	}

	Common::MemoryView::SizeType
	SerialPort
	::Write(const Common::MemoryView& view)
	{
		boost::system::error_code result{};
		auto bytes = write(*UniqueSerialPort, static_cast<boost::asio::mutable_buffer>(view), result);
		spdlog::info(
			"SerialPort({})> Write {}:{} bytes, failed({}), failure({})",
			Info.DeviceName,
			bytes,
			view.Size,
			result.failed(),
			RESULT_MESSAGE(result)
		);

		return bytes;
	}
}

#undef RESULT_MESSAGE
