#include <SangoC/Communication/Connections/SerialPort.hpp>

SangoC::Communication::Connections::SerialPort
::SerialPort(boost::asio::io_context& ioContext, SerialPortInfo info) :
	UniqueSerialPort(std::make_unique<boost::asio::serial_port>(ioContext)),
	Info(std::move(info))
{
}

void
SangoC::Communication::Connections::SerialPort
::ApplyOption(boost::system::error_code& result) const
{
	using AsioSerial = boost::asio::serial_port;

	UniqueSerialPort->set_option(AsioSerial::baud_rate(Info.BaudRate), result);
	if (result.failed()) return;

	UniqueSerialPort->set_option(AsioSerial::flow_control(AsioSerial::flow_control::none), result);
	if (result.failed()) return;

	UniqueSerialPort->set_option(AsioSerial::parity(AsioSerial::parity::none), result);
	if (result.failed()) return;

	UniqueSerialPort->set_option(AsioSerial::stop_bits(AsioSerial::stop_bits::one), result);
	if (result.failed()) return;

	UniqueSerialPort->set_option(AsioSerial::character_size(AsioSerial::character_size(Info.CharacterSize)), result);
}

void
SangoC::Communication::Connections::SerialPort
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
		spdlog::warn("SerialPort({})> Cannot open: {}", Info.DeviceName, ""/*result.what()*/);
		return;
	}

	ApplyOption(result);
	if (result.failed())
	{
		spdlog::warn("SerialPort({})> Cannot set option: {}", Info.DeviceName, ""/*result.what()*/);
		return;
	}

	Info.IsConnected = true;
}

std::shared_ptr<SangoC::Communication::Connections::SerialPort>
SangoC::Communication::Connections::SerialPort::
BuildShared(boost::asio::io_context& ioContext, const SerialPortInfo& info)
{
	spdlog::info("Building Shared SerialPort({})", info.ToString());
	auto ptr = std::shared_ptr<SerialPort>(new SerialPort{ioContext, info});
	spdlog::info("Finished building SerialPort({})", info.DeviceName);
	return ptr;
}

SangoC::Communication::Connections::SerialPort
::~SerialPort()
{
	spdlog::info("SerialPort({})> Trying closing", Info.DeviceName);
	Disconnect();
	spdlog::info("SerialPort({})> Closed", Info.DeviceName);
}

void
SangoC::Communication::Connections::SerialPort
::Connect()
{
	boost::system::error_code result;
	RefreshConnection(result);
	if (result.failed())
	{
		spdlog::warn("SerialPort({})> Cannot connect: {}", Info.DeviceName, ""/*result.what()*/);
	}
}

bool
SangoC::Communication::Connections::SerialPort
::IsConnected()
{
	return Info.IsConnected && UniqueSerialPort->is_open();
}

void
SangoC::Communication::Connections::SerialPort
::Disconnect()
{
	boost::system::error_code result;
	UniqueSerialPort->close(result);
	if (result.failed())
	{
		spdlog::warn("SerialPort({})> Cannot close serial port: {}", Info.DeviceName, ""/*result.what()*/);
	}

	Info.IsConnected = false;
}

const
SangoC::Communication::Connections::SerialPortInfo&
SangoC::Communication::Connections::SerialPort::GetInfo() const
{
	return Info;
}

SangoC::Memory::MemoryView::SizeType
SangoC::Communication::Connections::SerialPort
::Read(const Memory::MemoryView& view)
{
	boost::system::error_code result{};
	auto bytes = read(*UniqueSerialPort, view.ToBuffer(), result);
	spdlog::info(
		"SerialPort({})> Read {}:{} bytes, failed({}), failure({})",
		Info.DeviceName,
		bytes,
		view.Size,
		result.failed(),
		""/*result.what()*/
	);

	//TODO failure counter and exception processor

	return bytes;
}

SangoC::Memory::MemoryView::SizeType
SangoC::Communication::Connections::SerialPort
::Write(const Memory::MemoryView& view)
{
	boost::system::error_code result{};
	auto bytes = write(*UniqueSerialPort, view.ToBuffer(), result);
	spdlog::info(
		"SerialPort({})> Write {}:{} bytes, failed({}), failure({})",
		Info.DeviceName,
		bytes,
		view.Size,
		result.failed(),
		""/*result.what()*/
	);

	return bytes;
}

std::string
SangoC::Communication::Connections::SerialPort
::ToString()
{
	return fmt::format("SerialPort({})", Info.ToString());
}