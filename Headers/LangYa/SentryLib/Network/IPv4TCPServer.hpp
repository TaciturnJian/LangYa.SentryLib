#pragma once

#include <atomic>
#include <thread>
#include <functional>

#include <boost/asio/ip/tcp.hpp>

#include <LangYa/SentryLib/Network/IPv4TCPSocket.hpp>
#include <LangYa/SentryLib/ManagedStack.hpp>

namespace LangYa::SentryLib
{
	/// @brief 理论上能接收无限客户端连接的 IPv4 TCP 服务器。
	///	@details 实际不能接收无线客户端连接，而且你必须小心内存被这么多 TCP 连接占满。
	///	启动服务器后，阻塞当前线程，并且监听指定的端口。
	class IPv4TCPServer final
	{
	public:
		/// @brief 客户端处理器。
		///	@details 此函数需要能够处理包装后的客户端，在外部中断信号到来时能够保证在合适时退出。
		///	@param client 包装后的客户端，提供了 IO 操作和一些基本属性。
		using ClientHandler = 
			const std::function<void(IPv4TCPSocket& client, std::shared_ptr<std::atomic_bool> interruptSignal)>&;

	private:
		/// @brief 服务器的线程 Stack 类型，用来循环利用线程，在结束后正确释放线程。
		using ManagedStackType = ManagedStack<std::thread>;

		/// @brief 客户端线程信息。
		using ThreadInfoType = ManagedStackType::ItemInfo;

		/// @brief 服务器的线程 Stack，用来循环利用线程，在结束后正确释放线程。
		ManagedStackType ClientThreadStack{};

		/// @brief 监听的本地端口。
		IPv4Endpoint LocalEndPoint{};

		/// @brief Acceptor 的 IO 上下文。
		boost::asio::io_context AcceptorIOContext{};
		
		/// @brief 服务器的 Acceptor 。
		boost::asio::ip::tcp::acceptor Acceptor;

		/// @brief 指定当前是否能够接收下一个客户端。
		std::atomic_bool CanAcceptNextClient{false};

		/// @brief 指定当前是否正在释放资源。
		std::shared_ptr<std::atomic_bool> ReleaseSignal{std::make_shared<std::atomic_bool>(false)};

		/// @brief 处理接收客户端的事务。
		///	@param threadInfo 线程信息。
		///	@param handler 处理客户端的函数，在客户端连接后调用。
		///	@param interruptSignal 用于外部控制服务器的中断信号。
		void AcceptClient(
			const ThreadInfoType& threadInfo,
			ClientHandler handler,
			 std::shared_ptr<std::atomic_bool> interruptSignal
		);

	public:
		/// @brief 停止接收新客户端，关闭服务器，销毁所有资源。
		~IPv4TCPServer();

		/// @brief 初始化服务器，指定监听的地址，在执行 Run 之后正式开始监听。
		///	@param listenTarget 监听的地址。
		explicit IPv4TCPServer(IPv4Endpoint listenTarget);

		/// @brief 启动服务器，阻塞当前线程。
		///	@param interruptSignal 用于外部控制服务器的中断信号。
		///	中断信号会在服务器启动时轮询。
		///	服务器会在中断后逐个关闭所有连接，需要时间且仍然会阻塞当前线程。
		///	当服务器异常时，中断信号会被设置为 true ，然后服务器会尝试释放资源。
		///	@param newClientCallback 处理客户端连接的函数，在有新的客户端连接时会被调用。
		void Run(std::shared_ptr<std::atomic_bool> interruptSignal, ClientHandler newClientCallback);
	};
}
