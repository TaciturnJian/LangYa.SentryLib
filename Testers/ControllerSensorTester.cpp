#include <LangYa/SentryLib/Controller.hpp>
#include <LangYa/SentryLib/Sensor.hpp>
#include <LangYa/SentryLib/Configurator.hpp>
#include <LangYa/SentryLib/TCPConnection.hpp>
#include <LangYa/SentryLib/CSharpp.hpp>
#include <LangYa/SentryLib/ApplicationInfo.hpp>

LangYa::SentryLib::ApplicationInfo AppInfo{
	"ControllerSensorTester",
	"A tester for controller and sensor.",
	{1,0,0,0}
};

struct
{
	std::string ConfigFilePath{"./config.json"};
	int AStep{0};
	float BStep{0};
}
ApplicationArgumentTable;

bool MappingArguments(boost::json::value& json)
{
	#define SC_SAME_NAME_MAPPING(x) spdlog::info("Mapping> {} -> {}", #x, #x); ApplicationArgumentTable.x = json.at(#x).

	SC_SAME_NAME_MAPPING(AStep) as_int64();
	SC_SAME_NAME_MAPPING(BStep) as_double();

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

	if (!configurator.Load(ApplicationArgumentTable.ConfigFilePath, MappingArguments)) return -3;

	spdlog::info("Now AStep->{} BStep->{}", ApplicationArgumentTable.AStep, ApplicationArgumentTable.BStep);

	return 0;
}
