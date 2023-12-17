#include <LangYa/SentryLib/SerializableContentController.hpp>
#include <LangYa/SentryLib/DeserializableContentSensor.hpp>
#include <LangYa/SentryLib/SentryData.hpp>
#include <LangYa/SentryLib/Application/Configurator.hpp>
#include <LangYa/SentryLib/Common/TCPConnection.hpp>
#include <LangYa/SentryLib/Common/CSharpp.hpp>
#include <LangYa/SentryLib/Application/ApplicationInfo.hpp>

#include <LangYa/SentryLib/LangYaConnection.hpp>

LangYa::SentryLib::ApplicationInfo AppInfo{
	"ControllerSensorTester",
	"A tester for controller and sensor.",
	{1,0,0,0}
};

struct
{
	std::string ConfigFilePath{"./config.json"};
}
ApplicationArgumentTable;

bool MappingArguments(boost::json::value& json)
{
	#define SC_SAME_NAME_MAPPING(x) spdlog::info("Mapping> {} -> {}", #x, #x); ApplicationArgumentTable.x = json.at(#x).

	#undef SC_SAME_NAME_MAPPING

	return true;
}

SC_ENTRY_POINT
{
	using namespace LangYa::SentryLib;

	// [可选步骤] 初始化文件-控制台 双重日志器
	if (!Configurator::InitializeMultiLogger()) return -1;

	// [可选步骤] 打印程序的基本信息
	AppInfo.OutputTo_spdlog();

	// 第一步，使用Configurator让程序支持命令行和配置文件。
	// 首先先创建匿名类型（ApplicationArgumentTable）
	// 然后使用 AddOption 添加命令行内容
	Configurator configurator{};
	configurator.AddOption()
	("help", "Display command line argument information.")
	(
		"ConfigPath",
		boost::program_options::value<std::string>(&ApplicationArgumentTable.ConfigFilePath), 
		"Redefine file path for configuration."
	);

	// 第二步，读取命令行参数的变量
	if (!configurator.Load({argc, argv})) return -2;

	// 第三步，执行命令行参数的逻辑
	if (configurator("help"))
	{
		spdlog::info("{}> {}", AppInfo.Name, configurator.GetHelpContent());
		return 0;
	}

	/// 第四步，将配置文件中的数据读出到类型中
	if (!Configurator::Load(ApplicationArgumentTable.ConfigFilePath, MappingArguments)) return -3;

	boost::asio::io_service io_service;
	auto tcp_connection_ptr = TCPConnection::BuildShared(io_service, "127.0.0.1", 8989);
	const auto temp_sentry_data = SentryData{};
	auto lang_ya_connection_ptr = std::make_shared<LangYaConnection>(
		temp_sentry_data.GetDeserializationResourceSize(), 
		temp_sentry_data.GetSerializationResultSize(), 
		tcp_connection_ptr
	);
	auto sensor_ptr = std::make_shared<DeserializableContentSensor<SentryData>>(lang_ya_connection_ptr);
	auto controller_ptr = std::make_shared<SerializableContentController<SentryData>>(lang_ya_connection_ptr);

	using namespace std::chrono_literals;

	std::thread io_thread([lang_ya_connection_ptr]
	{
		while (true) {
			lang_ya_connection_ptr->Connect();
			if (!lang_ya_connection_ptr->IsConnected())
			{
				std::this_thread::sleep_for(1ms);
				continue;
			}
			lang_ya_connection_ptr->HandleIO();
		}
	});

	std::thread tm_thread([sensor_ptr, controller_ptr]
	{
		while (true)
		{
			std::this_thread::sleep_for(1ms);
			sensor_ptr->Tick();
			controller_ptr->Tick();
		}
	});

	std::thread op_thread([sensor_ptr, controller_ptr]
	{
		auto& controller = *controller_ptr;
		auto& sensor = *sensor_ptr;
		std::this_thread::sleep_for(1s);

		SentryData data;
		data.AmmoCount = 4000;
		while (true)
		{
			std::this_thread::sleep_for(1s);

			data.AmmoCount -= 1;
			controller.SetData(data);
		}
	});

	io_thread.join();
	tm_thread.join();
	op_thread.join();
	
	return 0;
}
