#pragma once
#include <functional>

#include <boost/asio.hpp>

namespace LangYa::SentryLib
{

	/// 服务器 接收来自客户端的连接
	///
	/// Acceptor <- C1
	///
	/// 新线程 处理有关C1的内容 (C1)
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
