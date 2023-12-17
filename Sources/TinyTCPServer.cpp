#include <LangYa/SentryLib/Common/CSharpp.hpp>
#include <LangYa/SentryLib/TinyTCPServer.hpp>

void LangYa::SentryLib::TinyTCPServer::WaitForClient(std::atomic_bool& terminated, std::atomic_bool& accepted)
{
	boost::asio::ip::tcp::socket* client_ptr = nullptr;
	// ReSharper disable once CppTooWideScope
	boost::asio::io_context io_context;
	try
	{
		accepted = false;
		client_ptr = new boost::asio::ip::tcp::socket(Acceptor.accept(io_context));
		accepted = true;
	}
	catch (...)
	{
		if (accepted != true)
		{
			accepted = true;
		}
	}

	try
	{
		if (client_ptr != nullptr)
		{
			Callback(*client_ptr);
		}
	}
	catch (...)
	{
		//TODO 异常处理
	}

	delete client_ptr;
	client_ptr = nullptr;

	terminated = false;
}

LangYa::SentryLib::TinyTCPServer::TinyTCPServer(ClientCallbackType callback):
	Callback(std::move(callback))
{
}

void LangYa::SentryLib::TinyTCPServer::Start(const std::atomic_bool& stopSignal)
{
	SC_TIME_TEXT;

	while (!stopSignal)
	{
		if (!NextThread)
		{
			if (AcceptedClient) NextThread = true;

			std::this_thread::sleep_for(100ms);
			continue;
		}

		NextThread = false;
		AcceptedClient = false;

		// 从已经创建的线程池中找一个已经终结的线程。
		bool found_terminated_thread{false};
		for (auto& thread_info : ThreadInfoList)
		{
			auto& terminated = *std::get<0>(thread_info);
			auto& shared_thread_ptr = std::get<1>(thread_info);
			if (!terminated) continue;

			// 如果找到，那么创建一个共享的线程指针，更新线程状态，并detach线程使其独立运行。
			found_terminated_thread = true;
			shared_thread_ptr = std::make_shared<std::thread>([&terminated, this]
			{
				WaitForClient(terminated, AcceptedClient);
			});
			shared_thread_ptr->detach();
			break;
		}

		if (found_terminated_thread) continue;

		ThreadInfoList.emplace_back(
			std::make_shared<std::atomic_bool>(false),
			std::make_shared<std::thread>()
		);
		auto& thread_info = ThreadInfoList[ThreadInfoList.size() - 1];
		auto& terminated = *std::get<0>(thread_info);
		auto& shared_thread_ptr = std::get<1>(thread_info);
		shared_thread_ptr = std::make_shared<std::thread>([&terminated, this]
		{
			WaitForClient(terminated, AcceptedClient);
		});
		shared_thread_ptr->detach();
	}
}
