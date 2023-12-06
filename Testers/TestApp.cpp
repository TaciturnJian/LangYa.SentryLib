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

int main(int argc, char** argv)
{
	AppInfo.OutputTo_spdlog();

	spdlog::info("{}> Parsing command line arguments", AppInfo.Name);
	Configurator configurator{};
	configurator.AddOption()
		("help","display all help")
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
