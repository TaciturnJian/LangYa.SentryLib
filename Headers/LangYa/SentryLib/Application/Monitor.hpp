#pragma once

#include <map>
#include <ranges>

#include <LangYa/SentryLib/Application/MonitorPackage.hpp>

namespace LangYa::SentryLib
{
	class Monitor
	{
		std::map<std::string, MonitorPackage> Packages{};

	public:
		Monitor& operator<<(const MonitorPackage& package)
		{
			Packages[package.Name] = package;
			return *this;
		}

		Monitor& operator<<(const std::string& json)
		{
			MonitorPackage package{"ErrorPackage"};
			package.ParseJson(json);

			return *this << package;
		}

		Monitor& operator>>(std::ostream& stream)
		{
			stream << "Monitor>\n\n";
			for (auto& package : Packages | std::views::values)
			{
				package.FormatToConsole(stream);
			}
			stream << "Monitor<\n";
			return *this;
		}
	};
}
