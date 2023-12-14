#pragma once
#include <functional>

#include <boost/asio.hpp>

namespace LangYa::SentryLib
{
	class TinyTCPServer
	{
	public:
		using ClientCallbackType = std::function<void(boost::asio::ip::tcp::socket& client)>;

	private:
		boost::asio::io_context IOContext;
		boost::asio::ip::tcp::acceptor Acceptor{
			IOContext,
			{boost::asio::ip::make_address("127.0.0.1"), 8989}
		};
		ClientCallbackType Callback;

		void WaitForClient(std::atomic_bool& terminated, std::atomic_bool& accepted);

		bool NextThread{true};
		std::atomic_bool AcceptedClient{false};
		std::vector<std::tuple<std::shared_ptr<std::atomic_bool>, std::shared_ptr<std::thread>>> ThreadInfoList;

	public:
		explicit TinyTCPServer(ClientCallbackType callback);

		void Start(const std::atomic_bool& stopSignal);
	};
}
