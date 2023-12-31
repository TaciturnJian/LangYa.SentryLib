#pragma once

#include <functional>

#include <boost/asio/ip/tcp.hpp>
#include <LangYa/SentryLib/Network/IPv4TCPSocket.hpp>


namespace LangYa::SentryLib
{
	class IPv4TCPServer
	{
	public:
		using SharedAcceptorType = std::shared_ptr<boost::asio::ip::tcp::acceptor>;

		struct ClientThreadInfo
		{
			/// @brief 指示目标线程是否完成任务，完成任务的线程可以销毁再换一个。
			std::shared_ptr<std::atomic_bool> IsTerminated{std::make_shared<std::atomic_bool>(true)};
			std::shared_ptr<std::thread> Thread{nullptr};
		};

		using ClientHandler = std::function<void(IPv4TCPSocket& client, const std::atomic_bool& interruptSignal)>;

	private:

		IPv4TCPSocket::SharedIOContextType SharedIOContext{nullptr};
		std::vector<ClientThreadInfo> ClientThreadInfos{};
		IPv4Endpoint LocalEndPoint{};
		SharedAcceptorType SharedAcceptorPtr{nullptr};
		std::atomic_bool CanAcceptNextClient{false};
		std::atomic_bool ReleaseSignal{false};

		void AcceptClient(const ClientThreadInfo& threadInfo, const ClientHandler& handler,
		                  const std::atomic_bool& interruptSignal);

		ClientThreadInfo& GetAvailableInfo();

	public:
		~IPv4TCPServer();

		explicit IPv4TCPServer(SharedAcceptorType acceptor);

		IPv4TCPServer(IPv4TCPSocket::SharedIOContextType sharedIOContext, IPv4Endpoint listenTarget);

		explicit IPv4TCPServer(IPv4Endpoint listenTarget);

		/// @brief 启动服务器，阻塞当前线程。
		///	@param interruptSignal 用于外部控制服务器的中断信号。
		///	中断信号会在服务器启动时轮询。
		///	服务器会在中断后逐个关闭所有连接，需要时间且仍然会阻塞当前线程。
		///	当服务器异常时，中断信号会被设置为 true ，然后服务器会尝试释放资源。
		///	@param handler 处理客户端连接的函数。在有新的客户端连接时会被调用。
		void Run(std::atomic_bool& interruptSignal, ClientHandler handler);
	};
}
