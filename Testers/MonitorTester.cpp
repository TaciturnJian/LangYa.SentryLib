#include <LangYa/SentryLib/Application/MonitorPackage.hpp>
#include <LangYa/SentryLib/Common/TCPConnection.hpp>

int main()
{
	using namespace std::chrono_literals;
	using namespace LangYa::SentryLib;

	// 使用 vector 的一些方法构造包
	MonitorPackage dynamic_package_1{"Test"};
	dynamic_package_1.Items.emplace_back("A", 9);
	dynamic_package_1.Items.emplace_back("B", 100);
	dynamic_package_1.Items.emplace_back("C", 100);

	// 使用直接构造
	MonitorPackage dynamic_package_2{"Test2",
		{			// 注意这里两层括号
			{"A", 9},
			{"B", 100},
			{"C", 100},
			{"D", 100}
		}
	};

	// 获取发送器
	boost::asio::io_context ioc;
	auto shared_connection = TCPConnection::BuildShared(ioc, "127.0.0.1", 8989);

	while (true)
	{
		// 发送包裹
		dynamic_package_1.WriteJsonToSharedConnection(shared_connection);
		dynamic_package_2.WriteJsonToSharedConnection(shared_connection);

		// 自己修改包裹
		dynamic_package_1.Items[1].Value++;
		dynamic_package_2.Items[2].Value++;

		// 建议频率不要太大，延时最低不要低于 10ms
		std::this_thread::sleep_for(50ms);
	}
}
