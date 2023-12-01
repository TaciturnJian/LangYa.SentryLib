#include <LangYa/SentryLib/TCPConnection.hpp>

#ifdef SC_WINDOWS
#define RESULT_MESSAGE(result) result.what()
#elif SC_LINUX
#define RESULT_MESSAGE(result) result.message()
#endif

namespace LangYa::SentryLib
{
	using IOContextType = boost::asio::io_context;
	using TCP = boost::asio::ip::tcp;
	using ErrorCodeType = boost::system::error_code;

	TCPConnection
	::TCPConnection(IOContextType& ioContext, TCP::endpoint remote) :
		Endpoint(std::move(remote)),
		RemoteEndpoint(fmt::format("{}:{}", remote.address().to_string(), remote.port())),
		Socket(std::make_unique<TCP::socket>(ioContext))
	{
	}

	void
	TCPConnection
	::RefreshConnection(ErrorCodeType& result)
	{
		spdlog::info("TCPConnection({})> Refreshing connection", RemoteEndpoint);
		Socket->connect(Endpoint, result);
		if (result.failed())
		{
			spdlog::warn("TCPConnection({})> Cannot connect: {}", RemoteEndpoint, RESULT_MESSAGE(result));
		}
	}

	std::shared_ptr<TCPConnection>
	TCPConnection
	::BuildShared(IOContextType& ioContext, std::string address, unsigned short port)
	{
		spdlog::info("Building Shared TCPConnection({}:{})", address, port);
		auto ptr = std::shared_ptr<TCPConnection>(new TCPConnection{
			ioContext,
			boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port)
		});
		spdlog::info("Finished building TCPConnection({}:{})", address, port);
		return ptr;
	}

	TCPConnection
	::~TCPConnection()
	{
		spdlog::info("TCPConnection({})> Trying closing", RemoteEndpoint);
		Disconnect();
		spdlog::info("TCPConnection({})> Closed", RemoteEndpoint);
	}

	void
	TCPConnection
	::Connect()
	{
		ErrorCodeType result;
		RefreshConnection(result);
		if (result.failed())
		{
			spdlog::warn("TCPConnection({})> Cannot connect: {}", RemoteEndpoint, RESULT_MESSAGE(result));
			Connected = false;
			return;
		}

		Connected = true;
	}

	bool
	TCPConnection
	::IsConnected()
	{
		return Connected;
	}

	void
	TCPConnection
	::Disconnect()
	{
		boost::system::error_code result;
		Socket->close(result);
		if (result.failed())
		{
			spdlog::warn("TCPConnection({})> Cannot close tcp connection: {}", RemoteEndpoint, RESULT_MESSAGE(result));
		}

		Connected = false;
	}

	MemoryView::SizeType
	TCPConnection
	::Read(const MemoryView& view)
	{
		ErrorCodeType result{};
		auto bytes = read(*Socket, view.ToBuffer(), result);
		spdlog::info(
			"TCPConnection({})> Read {}:{} bytes, failed({}), failure({})",
			RemoteEndpoint,
			bytes,
			view.Size,
			result.failed(),
			RESULT_MESSAGE(result)
		);

		//TODO failure counter and exception processor

		return bytes;
	}

	MemoryView::SizeType
	TCPConnection
	::Write(const MemoryView& view)
	{
		ErrorCodeType result{};
		auto bytes = write(*Socket, view.ToBuffer(), result);
		spdlog::info(
			"TCPConnection({})> Write {}:{} bytes, failed({}), failure({})",
			RemoteEndpoint,
			bytes,
			view.Size,
			result.failed(),
			RESULT_MESSAGE(result)
		);

		return bytes;
	}

	std::string
	TCPConnection
	::ToString()
	{
		return fmt::format("TCPConnection({})", RemoteEndpoint);
	}
}
#undef RESULT_MESSAGE
