#pragma once

#include <LangYa/Network/IClientAcceptor.hpp>
#include <LangYa/Network/IPv4TCPSocket.hpp>

namespace LangYa::Network
{
	class IPv4TCPAcceptor final : public IClientAcceptor<IPv4TCPSocket>
	{
		IPv4Endpoint Endpoint;
		boost::asio::io_context IOContext{};
		boost::asio::ip::tcp::acceptor Acceptor;
		std::string EndpointString;

	public:
		explicit IPv4TCPAcceptor(IPv4Endpoint endpoint);

		std::shared_ptr<IPv4TCPSocket> AcceptClient() override;
	};
}
