#include <LangYa/SentryLib/Network/IPv4TCPAcceptor.hpp>

namespace LangYa::SentryLib
{
	IPv4TCPAcceptor
	::IPv4TCPAcceptor(IPv4Endpoint endpoint):
		Endpoint(std::move(endpoint)),
		Acceptor(IOContext, static_cast<boost::asio::ip::tcp::endpoint>(Endpoint)),
		EndpointString(FormatToString(Endpoint))
	{
		spdlog::info("IPv4TCPAcceptor> Created acceptor on {}, trying start listening", EndpointString);
		Acceptor.listen();
		spdlog::info("IPv4TCPAcceptor> Listening {}", EndpointString);
	}

	std::shared_ptr<IPv4TCPSocket>
	IPv4TCPAcceptor
	::AcceptClient()
	{
		spdlog::info("IPv4TCPAcceptor> Waiting for new connection at {}", EndpointString);
		boost::system::error_code result{};
		auto io_context_pointer = std::make_shared<boost::asio::io_context>();
		auto shared_client = std::make_shared<boost::asio::ip::tcp::socket>(
			Acceptor.accept(*io_context_pointer, result));

		if (result.failed())
		{
			spdlog::error("IPv4TCPAcceptor> Failed to accept new connection at {}: {}", EndpointString, result.to_string());
			return nullptr;
		}

		auto client_pointer = std::make_shared<IPv4TCPSocket>(io_context_pointer, shared_client);
		std::stringstream stream{};
		client_pointer->GetTargetInfo(stream);
		auto remote_endpoint = stream.str();
		spdlog::info("IPv4TCPAcceptor> Accepted new connection at {} from {}", EndpointString, remote_endpoint);
		return client_pointer;
	}
}
