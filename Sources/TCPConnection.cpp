#include <LangYa/SentryLib/TCPConnection.hpp>

using IOContextType = boost::asio::io_context;
using TCP = boost::asio::ip::tcp;
using ErrorCodeType = boost::system::error_code;

LangYa::SentryLib::TCPConnection
::TCPConnection(IOContextType& ioContext, TCP::endpoint remote) :
	Endpoint(std::move(remote)),
	RemoteEndpoint(fmt::format("{}:{}", remote.address().to_string(), remote.port())),
	Socket(std::make_unique<TCP::socket>(ioContext))
{
}

void
LangYa::SentryLib::TCPConnection
::RefreshConnection(ErrorCodeType& result)
{
	spdlog::info("TCPConnection({})> Refreshing connection", RemoteEndpoint);
	Socket->connect(Endpoint, result);
	if (result.failed())
	{
		spdlog::warn("TCPConnection({})> Cannot connect: {}", RemoteEndpoint, result.what());
	}
}

std::shared_ptr<LangYa::SentryLib::TCPConnection>
LangYa::SentryLib::TCPConnection
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

LangYa::SentryLib::TCPConnection
::~TCPConnection()
{
	spdlog::info("TCPConnection({})> Trying closing", RemoteEndpoint);
	Disconnect();
	spdlog::info("TCPConnection({})> Closed", RemoteEndpoint);
}

void
LangYa::SentryLib::TCPConnection
::Connect()
{
	ErrorCodeType result;
	RefreshConnection(result);
	if (result.failed())
	{
		spdlog::warn("TCPConnection({})> Cannot connect: {}", RemoteEndpoint, result.what());
		Connected = false;
		return;
	}

	Connected = true;
}

bool
LangYa::SentryLib::TCPConnection
::IsConnected()
{
	return Connected;
}

void
LangYa::SentryLib::TCPConnection
::Disconnect()
{
	boost::system::error_code result;
	Socket->close(result);
	if (result.failed())
	{
		spdlog::warn("TCPConnection({})> Cannot close tcp connection: {}", RemoteEndpoint, result.what());
	}

	Connected = false;
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::TCPConnection
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
		result.what()
	);

	//TODO failure counter and exception processor

	return bytes;
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::TCPConnection
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
		result.what()
	);

	return bytes;
}

std::string
LangYa::SentryLib::TCPConnection
::ToString()
{
	return fmt::format("TCPConnection({})", RemoteEndpoint);
}
