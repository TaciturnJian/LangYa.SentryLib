#include <LangYa/SentryLib/Application/MonitorSender.hpp>

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
	MonitorSender sender{};

	while (true)
	{
		// 发送包裹
		sender << dynamic_package_1 << dynamic_package_2;

		// 自己修改包裹
		dynamic_package_1.Items[1].Value++;
		dynamic_package_2.Items[2].Value++;

		// 建议频率不要太大，延时最低不要低于 10ms
		std::this_thread::sleep_for(50ms);
	}
}
