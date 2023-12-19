#pragma once

#include <map>
#include <ranges>

#include <LangYa/SentryLib/Application/MonitorPackage.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一个用来格式化数据的显示器。
	///	通过右移 >> 运算符，可以将数据包传出到流中。
	///	通过左移 << 运算符，可以将数据包直接传入显示器，显示器会自动解析数据包并显示。
	class Monitor
	{
		std::map<std::string, MonitorPackage> Packages{};

		MonitorPackage MonitorInfo {
			"MonitorInfo",
			{
				{"Normal", 0},
				{"Error", 0},
				{"Total", 0},
			}
		};

		void ResetMonitorInfo()
		{
			MonitorInfo.Items[0].Value = 0;
			MonitorInfo.Items[1].Value = 0;
			MonitorInfo.Items[2].Value = 0;
		}

	public:
		Monitor& operator<<(const MonitorPackage& package)
		{
			Packages[package.Name] = package;
			return *this;
		}

		Monitor& operator<<(const std::string& json)
		{
			MonitorPackage package{"ErrorPackage"};
			package.ParseJson(json) ? ++MonitorInfo.Items[0].Value : ++MonitorInfo.Items[1].Value;

			if (++MonitorInfo.Items[2].Value > 10000)
			{
				ResetMonitorInfo();
			}

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
