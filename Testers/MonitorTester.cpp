#include <iostream>
#include <iomanip>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>
#include <fmt/format.h>

#include <LangYa/SentryLib/Common/CSharpp.hpp>
#include <LangYa/SentryLib/ConsoleManipulator.hpp>
#include <LangYa/SentryLib/Application/Monitor.hpp>

#include "LangYa/SentryLib.hpp"

#define SC_USE_REMOTE_MONITOR

SC_ENTRY_POINT
{
	SC_TIME_TEXT;
	SC_ULS;

	MonitorPackage dynamic_package_1{"Test"};
	dynamic_package_1.Items.emplace_back("A", 9);
	dynamic_package_1.Items.emplace_back("B", 100);
	dynamic_package_1.Items.emplace_back("C", 100);

	MonitorPackage dynamic_package_2{"Test2"};
	dynamic_package_2.Items.emplace_back("A", 9);
	dynamic_package_2.Items.emplace_back("B", 100);
	dynamic_package_2.Items.emplace_back("C", 100);
	dynamic_package_2.Items.emplace_back("D", 100);

	#ifdef SC_USE_REMOTE_MONITOR

	boost::asio::io_context io_context;
	const auto tcp = TCPConnection::BuildShared(io_context, "127.0.0.1", 8989);
	tcp->Connect();
	if (!tcp->IsConnected())
	{
		std::cout << "Cannot connected to remote monitor.";
		return -1;
	}

	while (true)
	{
		dynamic_package_1.WriteJsonToConnection(tcp);
		dynamic_package_2.WriteJsonToConnection(tcp);

		dynamic_package_1.Items[1].Value++;
		dynamic_package_2.Items[2].Value++;

		std::this_thread::sleep_for(100ms);
	}

	#else

	Monitor monitor;
	ConsoleManipulator manipulator;

	while (true)
	{
		manipulator.MoveCursorTo({0,0});
		std::this_thread::sleep_for(10ms);
		std::stringstream stream;
		dynamic_package_1.FormatToJson(stream);
		monitor << stream.str();

		stream.str("");
		dynamic_package_2.FormatToJson(stream);
		monitor << stream.str() >> std::cout;

		dynamic_package_1.Items[1].Value++;
		dynamic_package_2.Items[2].Value++;
	}

	#endif
}
