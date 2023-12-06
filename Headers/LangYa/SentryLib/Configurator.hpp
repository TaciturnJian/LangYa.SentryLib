#pragma once

#include <ostream>
#include <vector>
#include <string>
#include <sstream>

#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

namespace LangYa::SentryLib {
	class Configurator {
		boost::program_options::options_description OptionsWithDescription{};
		boost::program_options::variables_map VariablesMap{};

	public:
		Configurator() = default;
		Configurator(const Configurator& other) = delete;
		Configurator(Configurator&& other) = delete;
		Configurator& operator=(const Configurator& other) = delete;
		Configurator& operator=(Configurator&& other) = delete;

		boost::program_options::options_description_easy_init AddOption()
		{
			return OptionsWithDescription.add_options();
		}

		bool Load(const int argumentCount, const char* const * const argumentList)
		{
			try {
				store(parse_command_line(argumentCount, argumentList, OptionsWithDescription), VariablesMap);
				notify(VariablesMap);
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

	};
}
