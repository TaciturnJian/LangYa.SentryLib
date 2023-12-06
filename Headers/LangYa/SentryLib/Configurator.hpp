#pragma once

#include <ostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <istream>
#include <iostream>
#include <functional>

#include <boost/json.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/CommandLineArguments.hpp>

namespace LangYa::SentryLib
{
	class Configurator
	{
		boost::program_options::options_description OptionsWithDescription{};
		boost::program_options::variables_map VariablesMap{};

	public:
		using JsonMapper = std::function<bool(boost::json::value&)>;

		Configurator();
		Configurator(const Configurator& other) = delete;
		Configurator(Configurator&& other) = delete;
		Configurator& operator=(const Configurator& other) = delete;
		Configurator& operator=(Configurator&& other) = delete;

		boost::program_options::options_description_easy_init AddOption();

		bool Load(const CommandLineArguments& commandLineArguments);

		static bool Load(const std::string& jsonFilePath, const JsonMapper& mapper);
	};
}
