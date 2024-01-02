#pragma once

#include <atomic>
#include <thread>
#include <functional>

#include <boost/asio/ip/tcp.hpp>
#include <LangYa/SentryLib/Network/IPv4TCPSocket.hpp>

namespace LangYa::SentryLib
{
	/// @brief 理论上能接收无限客户都连接的 IPv4 TCP 服务器。
	///	@details 实际不能，而且你必须小心内存被这么多 TCP 连接占满。
	class IPv4TCPServer
	{
	public:
		/// @brief 内含两个指针，分别指向线程是否终止和线程对象。
		struct ClientThreadInfo
		{
			/// @brief 指示目标线程是否完成任务，完成任务的线程就可以销毁再换一个。
			std::shared_ptr<std::atomic_bool> IsTerminated{std::make_shared<std::atomic_bool>(true)};

			/// @brief 线程对象的指针。
			std::shared_ptr<std::thread> Thread{nullptr};
		};

		using SharedAcceptorType = std::shared_ptr<boost::asio::ip::tcp::acceptor>;
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
