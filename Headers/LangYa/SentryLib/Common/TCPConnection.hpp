#pragma once

#include <string>
#include <boost/asio.hpp>

#include <LangYa/SentryLib/Common/Connection.hpp>

namespace LangYa::SentryLib
{
	class TCPConnection final : public Connection
	{
		volatile bool Connected{false};
		boost::asio::ip::tcp::endpoint Endpoint;
		std::string RemoteEndpoint;
		std::unique_ptr<boost::asio::ip::tcp::socket> Socket;

		void RefreshConnection(boost::system::error_code& result);

	public:
		TCPConnection() = delete;

		TCPConnection(boost::asio::io_context& ioContext, boost::asio::ip::tcp::endpoint remote);

		static std::shared_ptr<TCPConnection> BuildShared(boost::asio::io_context& ioContext, std::string address,
		                                                  unsigned short port);

		explicit TCPConnection(boost::asio::ip::tcp::socket&& socket);

		~TCPConnection() override;

		void Connect() override;

		[[nodiscard]] bool IsConnected() override;

		void Disconnect() override;

		MemoryView::SizeType Read(const MemoryView& view) override;

		MemoryView::SizeType Write(const MemoryView& view) override;

		std::string ToString() override;
	};
}
