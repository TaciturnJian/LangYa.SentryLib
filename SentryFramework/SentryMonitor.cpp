#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include <LangYa/SentryLib/ConsoleManipulator.hpp>
#include <LangYa/SentryLib/Application/Monitor.hpp>
#include <LangYa/SentryLib/Common/TinyJsonStream.hpp>
#include <LangYa/SentryLib/TinyTCPServer.hpp>

#include <LangYa/SentryLib/Common/CSharpp.hpp>

using namespace boost::asio;

LangYa::SentryLib::ConsoleManipulator Manipulator{};
LangYa::SentryLib::Monitor GlobalMonitor{};

void HandleClient(ip::tcp::socket& client)
{
	char receive_buffer[2]{};
	LangYa::SentryLib::TinyJsonStream stream{
		[](const std::string& package)
		{
			GlobalMonitor << package;
		}
	};

	try
	{
		while (true)
		{
			if (read(client, buffer(receive_buffer, 1)) < 1) continue;
			stream << receive_buffer[0];
		}
	}
	catch (...)
	{
		//TODO 处理异常
	}
}

SC_ENTRY_POINT
{
	SC_TIME_TEXT;

	std::thread{
		[]
		{
			Manipulator.ClearScreen().HideCursor();
			while (true)
			{
				Manipulator.MoveCursorTo({0, 0});
				GlobalMonitor >> std::cout;
			}
		}
	}.detach();

	std::atomic_bool stop_signal{false};
	LangYa::SentryLib::TinyTCPServer server{HandleClient};
	server.Start(stop_signal);

	return 0;
}
