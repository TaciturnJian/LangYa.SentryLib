#pragma once

#include <LangYa/SentryLib/Network/IClientAcceptor.hpp>
#include <LangYa/SentryLib/Network/IPv4TCPSocket.hpp>
#include <spdlog/spdlog.h>

namespace LangYa::SentryLib
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
