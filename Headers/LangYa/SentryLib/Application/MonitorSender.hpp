#pragma once

#include <iostream>

#include <LangYa/SentryLib/Common/TCPConnection.hpp>
#include <LangYa/SentryLib/Application/MonitorPackage.hpp>

namespace LangYa::SentryLib
{
	class MonitorSender
	{
	public:
		inline static boost::asio::io_context IOContext{};
		inline static auto SharedTCP{TCPConnection::BuildShared(IOContext, "127.0.0.1", 8989)};
		MonitorSender()
		{
			try
			{
				auto& tcp = *SharedTCP;
				tcp.Connect();

				if (!tcp.IsConnected())
				{
					std::cout << "MonitorSender> Failed to connect to remote monitor";
					throw std::exception(); //TODO 指明原因是连接失败
				}
			}
			catch (std::exception& ex)
			{
				std::cout << "MonitorSender> Failed to build: " << ex.what();
			}
			catch (...)
			{
				std::cout << "MonitorSender> Failed to build, maybe log will be helpful";
			}
		}

		MonitorSender& operator<<(const MonitorPackage& package)
		{
			package.WriteJsonToConnection(SharedTCP);
			return *this;
		}
	};
}
