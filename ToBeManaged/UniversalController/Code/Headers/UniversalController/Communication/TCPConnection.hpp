#pragma once

#include <string>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include <SangoC/Communication/Connections/Connection.hpp>

namespace SangoC::Communication::Connections::Network
{
	class TCPConnection final : public Connection
	{
		volatile bool Connected{false};
		boost::asio::ip::tcp::endpoint Endpoint;
		std::string RemoteEndpoint;
		std::unique_ptr<boost::asio::ip::tcp::socket> Socket;

		TCPConnection(boost::asio::io_context& ioContext, boost::asio::ip::tcp::endpoint remote);

		void RefreshConnection(boost::system::error_code& result);

	public:
		TCPConnection() = delete;

		static std::shared_ptr<TCPConnection> BuildShared(boost::asio::io_context& ioContext, std::string address,
		                                                  unsigned port);

		~TCPConnection() override;

		void Connect() override;

		[[nodiscard]] bool IsConnected() override;

		void Disconnect() override;

		Memory::MemoryView::SizeType Read(const Memory::MemoryView& view) override;

		Memory::MemoryView::SizeType Write(const Memory::MemoryView& view) override;

		std::string ToString() override;
	};
}
