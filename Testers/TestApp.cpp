#include <LangYa/SentryLib.hpp>

using namespace LangYa::SentryLib;

ApplicationInfo AppInfo{
	"TestApp",
	"An application for tests in sentry lib.",
	{1, 0, 0, 0}
};
std::string JsonFilePath{"./config.json"};
long long VarA = 0;

bool MappingJsonToVariables(boost::json::value& json)
{
	VarA = json.at("VarA").as_int64();
	return true;
}

constexpr auto MaxLogFileSize = sizeof(char) * 1024 * 1024 * 5;
constexpr auto MaxLogFileCount = 10;

bool InitializeLogger()
{
	try
	{
		std::vector<spdlog::sink_ptr> sinks{};
		sinks.push_back(
			std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
				"logs/basic-log.txt", MaxLogFileSize, MaxLogFileCount
			)
		);
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		const auto logger = std::make_shared<spdlog::logger>("multi-logger", std::begin(sinks), std::end(sinks));
		set_default_logger(logger);
		return true;
	}
	catch (...)
	{
		std::cout << "Error in logging!";
		return false;
	}
}

int main(int argc, char** argv)
{
	if (!InitializeLogger()) return -1;

	AppInfo.OutputTo_spdlog();

	spdlog::info("{}> Parsing command line arguments", AppInfo.Name);
	Configurator configurator{};
	configurator.AddOption()
		("help", "display all help")
		(
			"ConfigPath",
			boost::program_options::value(&JsonFilePath),
			"File path for the configuration file. May be the absolute path will be better"
		);
	if (!configurator.Load({argc, argv}))
	{
		spdlog::error("{}> Failed to parse command line arguments", AppInfo.Name);
		return -1;
	}
	if (configurator("help"))
	{
		spdlog::info("{}> {}", AppInfo.Name, configurator.GetHelpContent());
		return 0;
	}

	if (!Configurator::Load(JsonFilePath, MappingJsonToVariables))
	{
		spdlog::error("{}> Failed to load configuration file", AppInfo.Name);
		return -1;
	}

	spdlog::info("{}> Value for VarA is: {}", AppInfo.Name, VarA);

	return 0;
}
