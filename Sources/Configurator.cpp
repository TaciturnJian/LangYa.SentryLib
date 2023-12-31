#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <LangYa/SentryLib/Application/Configurator.hpp>

/* 上面这一堆头文件真的是一个正常的 .cpp 应该有的东西吗？ */

namespace LangYa::SentryLib
{
	Configurator
	::Configurator() = default;

	boost::program_options::options_description_easy_init
	Configurator
	::AddOption()
	{
		return OptionsWithDescription.add_options();
	}

	bool
	Configurator
	::operator()(const std::string& flag) const
	{
		// ReSharper disable once CppUseAssociativeContains
		return VariablesMap.count(flag) > 0;
	}

	bool
	Configurator
	::Load(const CommandLineArguments& commandLineArguments)
	{
		try /* 可能会报错，所以使用了 try-catch，具体报什么错，我不想测 (￢︿￢) */
		{
			spdlog::info("Configurator> Begin parsing command line arguments.");
			store(
				parse_command_line(static_cast<int>(commandLineArguments.ArgumentCount),
				                   commandLineArguments.ArgumentList,
				                   OptionsWithDescription),
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

		/* try catch catch 每个分支都有 return，这代码谁写的 */
	}

	std::string
	Configurator
	::GetHelpContent() const
	{
		/* 原来这里用的 fmt，代码可好看了，现在换成了 stream */
		/* 谁要可读性啊，cpp 程序员看不得一点性能损失 (●'◡'●) */
		std::stringstream stream{"Configurator> Command Line Argument Help Content:\n"};
		stream << OptionsWithDescription;
		return stream.str();
	}

	bool
	Configurator
	::Load(const std::string& jsonFilePath, const JsonMapper& mapper)
	{
		spdlog::info("Configurator> Loading json file({})", jsonFilePath);
		std::ifstream file{};
		try
		{
			file.open(jsonFilePath, std::ios::in);
		}
		catch (const std::exception& ex)
		{
			spdlog::error(
				"Configurator> Cannot open json file({}) to read: {}",
				jsonFilePath,
				ex.what()
			);
			return false;
		}
		catch (...)
		{
			spdlog::error(
				"Configurator> Cannot open json file({}) to read: unknown exception ($2)",
				jsonFilePath
			);
			return false;
		}

		spdlog::info("Configurator> Parsing content from json file({})", jsonFilePath);
		boost::json::value json{};
		try
		{
			json = boost::json::parse(file);
		}
		catch (const std::exception& ex)
		{
			spdlog::error(
				"Configurator> Cannot parse json file({}): {}",
				jsonFilePath,
				ex.what()
			);
			return false;
		}
		catch (...)
		{
			spdlog::error(
				"Configurator> Cannot parse json file({}): unknown exception ($2)",
				jsonFilePath
			);
			return false;
		}

		spdlog::info("Configurator> Closing json file({})", jsonFilePath);
		try
		{
			/* 按道理，我完全不需要 close 这个文件，因为 ifsream 回收时会自己管理 */
			/* 但是我就是想 close，因为我想多写几句写 try catch (*^_^*)  */
			file.close();
		}
		catch (const std::exception& ex)
		{
			spdlog::error(
				"Configurator> Cannot close json file({}): {}",
				jsonFilePath,
				ex.what()
			);
			return false;
		}
		catch (...)
		{
			spdlog::error(
				"Configurator> Cannot close json file({}): unknown exception ($3)",
				jsonFilePath
			);
			return false;
		}

		spdlog::info("Configurator> Mapping json from file({})", jsonFilePath);
		try
		{
			if (!mapper(json))
			{
				spdlog::error("Configurator> Error in json mapping");
				return false;
			}
		}
		catch (const std::exception& ex)
		{
			spdlog::error("Configurator> Error in json mapping: {}", ex.what());
			return false;
		}
		catch (...)
		{
			spdlog::error("Configurator> Error in json mapping: unknown exception ($4)");
			return false;
		}

		return true;
	}

	bool
	Configurator
	::InitializeMultiLogger()
	{
		try
		{
			constexpr auto max_log_file_size = sizeof(char) * 1024 * 1024 * 5;
			constexpr auto max_log_file_count = 10;

			std::vector<spdlog::sink_ptr> sinks{};
			sinks.push_back(
				std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
					"logs/basic-log.txt", max_log_file_size, max_log_file_count
				)
			);
			sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			const auto logger = std::make_shared<
				spdlog::logger>("multi-logger", std::begin(sinks), std::end(sinks));
			set_default_logger(logger);
			return true;
		}
		catch (...)
		{
			std::cout << "Error in logging!";
			return false;
		}
	}
}
