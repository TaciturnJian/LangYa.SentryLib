#pragma once

#include <functional>

#include <boost/asio.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一个简单的 TCP 服务器。
	///	@details TinyTCPServer 是一个理论上能响应无限 TCP 客户端的服务器。
	///	相信我，你不会想知道这个的实现源码有多脏。 (QwQ)
	class TinyTCPServer
	{
	public:
		using ClientCallbackType = std::function<void(boost::asio::ip::tcp::socket& client)>;

		struct SharedThreadWithSharedTerminateSignal
		{
			std::shared_ptr<std::atomic_bool> IsTerminatedPtr;
			std::shared_ptr<std::thread> ThreadPtr;
		};

	private:
		/// @brief Acceptor 的 IOContext 。
		boost::asio::io_context IOContext{};
		boost::asio::ip::tcp::acceptor Acceptor{
			IOContext,
			{
				boost::asio::ip::make_address("127.0.0.1"),
				8989
			}
		};
		ClientCallbackType Callback;

		bool NextThread{true};
		std::atomic_bool AcceptedClient{false};
		std::vector<std::tuple<std::shared_ptr<std::atomic_bool>, std::shared_ptr<std::thread>>> ThreadInfoList;

		void WaitForClient(std::atomic_bool& terminated, std::atomic_bool& accepted);

	public:
		explicit TinyTCPServer(ClientCallbackType callback);

		~TinyTCPServer()
		{
			//开始终止所有线程分配任务
			AcceptedClient = false;
			NextThread = false;

			boost::system::error_code result{};

			// 关闭 Acceptor
			result = Acceptor.cancel(result);
			if (result.failed())
			{
				//TODO 异常处理
			}

			result = Acceptor.close(result);
			if (result.failed())
			{
				//TODO 异常处理
			}

			for (auto& thread_info: ThreadInfoList)
			{
				auto& shared_terminated = std::get<0>(thread_info);
				auto& shared_thread = std::get<1>(thread_info);

				*shared_terminated = false;	//设置为 false 以阻止进一步的线程分配

				//TODO 
				// 如何等待 thread 退出？
			}
		}

		void Start(const std::atomic_bool& stopSignal);
	};
}
