#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include <LangYa/SentryLib/ConsoleManipulator.hpp>
#include <LangYa/SentryLib/Application/Monitor.hpp>

using namespace boost::asio;

io_context IOContext;
ip::tcp::acceptor Acceptor{IOContext, {ip::make_address("127.0.0.1"), 8989}};
LangYa::SentryLib::ConsoleManipulator Manipulator{};
LangYa::SentryLib::Monitor GlobalMonitor{};

void HandleClient(ip::tcp::socket& client)
{
	char receive_buffer[2]{};
	std::stringstream json_buffer{};
	unsigned int json_level{0};
	bool in_package{false};
	bool received_package{false};

	try
	{
		while (true)
		{
			if (received_package)
			{
				GlobalMonitor << json_buffer.str();
				json_buffer.str("");
				received_package = false;
				in_package = false;
			}

			if (read(client, buffer(receive_buffer, 1)) < 1) continue;

			const auto& ch = receive_buffer[0];
			switch (ch)
			{
			case '{':
				if (json_level == 0)
					in_package = true;

				json_level++;
				break;

			case '}':
				if (json_level == 1)
					received_package = true;

				if (json_level == 0)
					//TODO 处理异常
					break;

				json_level--;

				break;
			}

			if (in_package)
			{
				json_buffer << ch;
			}
		}
	}
	catch (...)
	{
		//TODO 处理异常
	}
}

void WaitForClient(std::atomic_bool& terminated, std::atomic_bool& accepted)
{
	ip::tcp::socket* client_ptr = nullptr;
	// ReSharper disable once CppTooWideScope
	io_context io_context;
	try
	{
		accepted = false;
		client_ptr = new ip::tcp::socket(Acceptor.accept(io_context));
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
			HandleClient(*client_ptr);
		}

		delete client_ptr;
	}
	catch (...)
	{
		client_ptr = nullptr;
	}

	terminated = false;
}
 
SC_ENTRY_POINT
{
	SC_TIME_TEXT;

	std::thread{[]
	{
		Manipulator.ClearScreen().HideCursor();
		while(true)
		{
			Manipulator.MoveCursorTo({0,0});
			GlobalMonitor >> std::cout;
		}
	}}.detach();

	bool next_thread{true};
	std::atomic_bool accepted_client{false};
	std::vector<std::tuple<std::shared_ptr<std::atomic_bool>, std::shared_ptr<std::thread>>> thread_info_list;
	while (true)
	{
		if (!next_thread)
		{
			if (accepted_client) next_thread = true;

			std::this_thread::sleep_for(100ms);
			continue;
		}

		next_thread = false;
		accepted_client = false;
		bool found_terminated_thread = false;
		for (auto& thread_info: thread_info_list)
		{
			auto& terminated = *std::get<0>(thread_info);
			auto& shared_thread_ptr = std::get<1>(thread_info);
			if (!terminated)
			{
				continue;
			}

			found_terminated_thread = true;
			shared_thread_ptr = std::make_shared<std::thread>([&terminated, &accepted_client]{WaitForClient(terminated, accepted_client);});
			shared_thread_ptr->detach();
			break;
		}

		if (!found_terminated_thread)
		{
			thread_info_list.emplace_back(std::make_shared<std::atomic_bool>(false), std::make_shared<std::thread>());
			auto& thread_info = thread_info_list[thread_info_list.size() - 1];
			auto& terminated = *std::get<0>(thread_info);
			auto& shared_thread_ptr = std::get<1>(thread_info);
			shared_thread_ptr = std::make_shared<std::thread>([&terminated, &accepted_client]{WaitForClient(terminated, accepted_client);});
			shared_thread_ptr->detach();
		}
	}
}
