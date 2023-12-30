#pragma once

#include <functional>

#include <boost/asio/ip/tcp.hpp>
#include <LangYa/SentryLib/IPv4TCPSocket.hpp>


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

		using ClientHandler = std::function<void(IPv4TCPSocket& client)>;

	private:
		IPv4TCPSocket::SharedIOContextType SharedIOContext{nullptr};
		std::vector<ClientThreadInfo> ClientThreadInfos{};
		IPv4Endpoint LocalEndPoint{};
		SharedAcceptorType SharedAcceptorPtr{nullptr};
		std::atomic_bool CanAcceptNextClient{false};

		ClientThreadInfo& GetAvailableInfo()
		{
			for (auto& info : ClientThreadInfos)
			{
				if (info.IsTerminated)
				{
					return info;
				}
			}

			ClientThreadInfos.emplace_back();
			return ClientThreadInfos.back();
		}

	public:
		explicit IPv4TCPServer(SharedAcceptorType acceptor) :
			SharedAcceptorPtr(std::move(acceptor))
		{
			LocalEndPoint = SharedAcceptorPtr->local_endpoint();
		}

		IPv4TCPServer(IPv4TCPSocket::SharedIOContextType sharedIOContext, IPv4Endpoint listenTarget) :
			SharedIOContext(std::move(sharedIOContext)),
			LocalEndPoint(std::move(listenTarget)),
			SharedAcceptorPtr(
				std::make_shared<boost::asio::ip::tcp::acceptor>(
					*SharedIOContext,
					LocalEndPoint.ToBoostTCPEndPoint()
				)
			)
		{
		}

		explicit IPv4TCPServer(IPv4Endpoint listenTarget) :
			SharedIOContext(std::make_shared<boost::asio::io_context>()),
			LocalEndPoint(std::move(listenTarget)),
			SharedAcceptorPtr(
				std::make_shared<boost::asio::ip::tcp::acceptor>(
					*SharedIOContext,
					LocalEndPoint.ToBoostTCPEndPoint()
				)
			)
		{
		}

		/// @brief 启动服务器
		///	@param interruptSignal 用于外部控制服务器的中断信号，中断信号会在服务器启动时轮询。服务器会在中断后逐个关闭所有连接，需要时间且仍然会阻塞当前线程。
		void Run(std::atomic_bool& interruptSignal)
		{
			if (interruptSignal)
			{
				spdlog::warn("IPv4TCPServer> Interrupt signal activated when server is starting.");
				return;
			}

			auto local_endpoint = FormatToConsoleFriendlyString(LocalEndPoint);
			spdlog::debug("IPv4TCPServer> Server is starting on {}.", local_endpoint);

			boost::system::error_code result{};
			(void)SharedAcceptorPtr->listen(boost::asio::socket_base::max_listen_connections, result);

			// 进入客户端循环，好戏现在才开始 qwq

			try
			{
				CanAcceptNextClient = true;
				while (!interruptSignal)
				{
					using namespace std::chrono_literals;

					if (!CanAcceptNextClient)
					{
						// 休眠 0.1s 降低 CPU 占用率
						std::this_thread::sleep_for(100ms);
						continue;
					}

					// 可以接收新的客户端了
					CanAcceptNextClient = false;
					auto& client_thread_info = GetAvailableInfo();
				}
			}
			catch (const std::exception& ex)
			{
				spdlog::error("IPv4TCPServer> Cannot hold up running: {}", ex.what());
				interruptSignal = true;
			}
			catch (...)
			{
				spdlog::error("IPv4TCPServer> Cannot hold up running: Unknown exception");
				interruptSignal = true;
			}

			CanAcceptNextClient = false;
		}
	};
}
