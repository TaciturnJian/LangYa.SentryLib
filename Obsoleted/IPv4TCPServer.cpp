#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Network/IPv4TCPServer.hpp>
#include <utility>

namespace LangYa::SentryLib
{
	IPv4TCPServer
	::~IPv4TCPServer()
	{
		// 关闭线程处理
		CanAcceptNextClient = false;
		ReleaseSignalPtr->store(true);
		auto local_endpoint = FormatToString(LocalEndPoint);
		spdlog::info("IPv4Server> Shutting down server at ({})", local_endpoint);

		// 释放资源
		Acceptor.cancel();
		Acceptor.close();
	}

	void
	IPv4TCPServer
	::AcceptClient(
		const ThreadInfoType& threadInfo, 
		ClientHandler handler,
	    std::shared_ptr<std::atomic_bool> interruptSignalPtr
	)
	{
		boost::asio::io_context client_io_context{};
		std::shared_ptr<IPv4TCPSocket> client_socket{nullptr};
		try
		{
			client_socket = std::make_shared<IPv4TCPSocket>(
				std::make_shared<boost::asio::ip::tcp::socket>(
					Acceptor.accept(client_io_context)
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

		if (! *ReleaseSignalPtr && client_socket != nullptr)
		{
			std::stringstream stream{};
			client_socket->GetTargetInfo(stream);
			spdlog::info("IPv4Server> Build client connection from ({}), calling client handler", stream.str());
			try
			{
				handler(*client_socket, std::move(interruptSignalPtr));
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
		threadInfo.Available->store(true);
	}

	IPv4TCPServer::IPv4TCPServer(IPv4Endpoint listenTarget):
		LocalEndPoint(std::move(listenTarget)),
		Acceptor(
			AcceptorIOContext,
			static_cast<boost::asio::ip::tcp::endpoint>(LocalEndPoint)
		)
	{
	}

	void IPv4TCPServer::Run(std::shared_ptr<std::atomic_bool> interruptSignalPtr, ClientHandler newClientCallback)  // NOLINT(performance-unnecessary-value-param)
	{
		if (*interruptSignalPtr)	// 检查中断信号
		{
			spdlog::warn("IPv4TCPServer> Interrupt signal activated when server is starting.");
			return;
		}

		auto local_endpoint = FormatToString(LocalEndPoint);
		spdlog::info("IPv4TCPServer> Server is starting on {}.", local_endpoint);

		boost::system::error_code result{};
		(void)Acceptor.listen(boost::asio::socket_base::max_listen_connections, result);

		// 进入客户端循环，好戏现在才开始
		try
		{
			CanAcceptNextClient = true;
			while (! *interruptSignalPtr && ! *ReleaseSignalPtr)
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
				auto& client_thread_info = ClientThreadStack.GetAvailableItem();
				client_thread_info.Available->store(false);
				client_thread_info.Item = std::make_shared<std::thread>(
					[this, &client_thread_info, &newClientCallback, interruptSignalPtr]()
					{
						AcceptClient(client_thread_info, newClientCallback, interruptSignalPtr);
					}
				);
				client_thread_info.Item->detach();
			}
		}
		catch (const std::exception& ex)
		{
			spdlog::error("IPv4TCPServer> Cannot hold up running: {}", ex.what());
			interruptSignalPtr->store(true);
		}
		catch (...)
		{
			spdlog::error("IPv4TCPServer> Cannot hold up running: Unknown exception");
			interruptSignalPtr->store(true);
		}

		CanAcceptNextClient = false;

		spdlog::info("IPv4TCPServer> Server at {} is stopped.", local_endpoint);
	}
}
