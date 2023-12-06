#include <LangYa/SentryLib/Configurator.hpp>

LangYa::SentryLib::Configurator
::Configurator() = default;

boost::program_options::options_description_easy_init
LangYa::SentryLib::Configurator
::AddOption()
{
	return OptionsWithDescription.add_options();
}

bool
LangYa::SentryLib::Configurator
::Load(const CommandLineArguments& commandLineArguments)
{
	try {
		spdlog::info("Configurator> Begin parsing command line arguments.");
		store(
			parse_command_line(static_cast<int>(commandLineArguments.ArgumentCount), commandLineArguments.ArgumentList, OptionsWithDescription), 
			VariablesMap
		);
		notify(VariablesMap);
		spdlog::info("Configurator> Finished parsing command line arguments.");

		return true;
	}
	catch (const std::exception& ex)
	{
		spdlog::error("Configurator> Caught exception in loading arguments: {}", ex.what());
		return false;
	}
	catch (...)
	{
		spdlog::error("Configurator> Caught exception in loading arguments: unknown exception ($1)");
		return false;
	}
}

bool
LangYa::SentryLib::Configurator
::Load(const std::string& jsonFilePath, const JsonMapper& mapper)
{
	std::ifstream file{};

	try {
		file.open(jsonFilePath, std::ios::in);
	}
	catch (const std::exception& ex)
	{
		spdlog::error("Configurator> Cannot open json file({}) to read: {}", jsonFilePath, ex.what());
		return false;
	}
	catch (...)
	{
		spdlog::error("Configurator> Cannot open json file({}) to read: unknown exception ($2)", jsonFilePath);
		return false;
	}

	boost::json::value json{};
	try
	{
		json = boost::json::parse(file);
	}
	catch (const std::exception& ex)
	{
		spdlog::error("Configurator> Cannot parse json file({}): {}", jsonFilePath, ex.what());
		return false;
	}
	catch (...)
	{
		spdlog::error("Configurator> Cannot parse json file({}): unknown exception ($2)", jsonFilePath);
		return false;
	}

	file.close();
	spdlog::info("Configurator> Closed json file({})", jsonFilePath);

	try
	{
		if (!mapper(json))
		{
			spdlog::error("Configurator> Error in json mapping");
			return false;
		}
		return true;
	}
	catch (const std::exception& ex)
	{
		spdlog::error("Configurator> Error in json mapping: {}", ex.what());
		return false;
	}
	catch (...)
	{
		spdlog::error("Configurator> Error in json mapping: unknown exception ($3)");
		return false;
	}
}