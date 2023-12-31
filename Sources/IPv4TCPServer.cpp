#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Network/IPv4TCPServer.hpp>

namespace LangYa::SentryLib
{
	IPv4TCPServer::~IPv4TCPServer()
	{
		// 关闭线程处理
		CanAcceptNextClient = false;
		ReleaseSignal = true;
		auto local_endpoint = FormatToConsoleFriendlyString(LocalEndPoint);
		spdlog::info("IPv4Server> Shutting down server at ({})", local_endpoint);

		// 释放资源
		ReleaseSignal = true;
		for (const auto& [IsTerminated, _] : ClientThreadInfos)
		{
			IsTerminated->store(false); // 阻止进一步线程分配
		}

		SharedAcceptorPtr->close();
	}

	void IPv4TCPServer::AcceptClient(const ClientThreadInfo& threadInfo, const ClientHandler& handler,
	                                 const std::atomic_bool& interruptSignal)
	{
		boost::asio::io_context client_io_context{};
		std::shared_ptr<IPv4TCPSocket> client_socket{nullptr};
		try
		{
			client_socket = std::make_shared<IPv4TCPSocket>(
				std::make_shared<boost::asio::ip::tcp::socket>(
					SharedAcceptorPtr->accept(client_io_context)
				)
			);
		}
		catch (const std::exception& ex)
		{
			spdlog::error("IPv4TCPServer> Cannot accept client: {}", ex.what());
			return;
		}
		catch (...)
		{
			spdlog::error("IPv4TCPServer> Cannot accept client: Unknown exception");
			return;
		}

		CanAcceptNextClient = true;

		if (!ReleaseSignal && client_socket != nullptr)
		{
			std::stringstream stream{};
			client_socket->GetTargetInfo(stream);
			spdlog::info("IPv4Server> Build client connection from ({}), calling client handler", stream.str());
			try
			{
				handler(*client_socket, interruptSignal);
			}
			catch (const std::exception& ex)
			{
				spdlog::error("IPv4TCPServer> Client handler threw an exception: {}", ex.what());
			}
			catch (...)
			{
				spdlog::error("IPv4TCPServer> Client handler threw an unknown exception");
			}
		}

		client_socket->Close();
		threadInfo.IsTerminated->store(true);
	}

	IPv4TCPServer::ClientThreadInfo& IPv4TCPServer::GetAvailableInfo()
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

	IPv4TCPServer::IPv4TCPServer(SharedAcceptorType acceptor):
		SharedAcceptorPtr(std::move(acceptor))
	{
		LocalEndPoint = SharedAcceptorPtr->local_endpoint();
	}

	IPv4TCPServer::IPv4TCPServer(IPv4TCPSocket::SharedIOContextType sharedIOContext,
	                             IPv4Endpoint listenTarget):
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

	IPv4TCPServer::IPv4TCPServer(IPv4Endpoint listenTarget):
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

	void IPv4TCPServer::Run(std::atomic_bool& interruptSignal, ClientHandler handler)
	{
		if (interruptSignal)
		{
			spdlog::warn("IPv4TCPServer> Interrupt signal activated when server is starting.");
			return;
		}

		auto local_endpoint = FormatToConsoleFriendlyString(LocalEndPoint);
		spdlog::info("IPv4TCPServer> Server is starting on {}.", local_endpoint);

		boost::system::error_code result{};
		(void)SharedAcceptorPtr->listen(boost::asio::socket_base::max_listen_connections, result);

		// 进入客户端循环，好戏现在才开始 qwq

		try
		{
			CanAcceptNextClient = true;
			while (!interruptSignal && !ReleaseSignal)
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
				client_thread_info.IsTerminated->store(false);
				client_thread_info.Thread = std::make_shared<std::thread>(
					[this, &client_thread_info, &handler, &interruptSignal]()
					{
						AcceptClient(client_thread_info, handler, interruptSignal);
					}
				);
				client_thread_info.Thread->detach();
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

		spdlog::info("IPv4TCPServer> Server at {} is stopped.", local_endpoint);
	}
}
