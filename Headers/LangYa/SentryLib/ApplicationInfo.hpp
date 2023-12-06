#pragma once

#include <string>
#include <ostream>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/SerialNumbers.hpp>

namespace LangYa::SentryLib
{
	struct ApplicationInfo final
	{
		std::string Name{"ApplicationName"};
		std::string Description{"ApplicationDescription"};
		SerialNumbers<unsigned short> Version{1, 0, 0, 0};

		[[nodiscard]] std::string ToString() const
		{
			std::stringstream stream{};
			stream << *this;
			return stream.str();
		}

		friend std::ostream& operator<<(std::ostream& stream, const ApplicationInfo& info)
		{
			return stream << R"({"Name":")" << info.Name << R"(","Description":")" << info.Description <<
				R"(","Version":)" << info.Version << R"("})";
		}

		// ReSharper disable once CppInconsistentNaming
		void OutputTo_spdlog() const
		{
			spdlog::info("Application: ");
			spdlog::info("  - Name: {}", Name);
			spdlog::info("  - Description: {}", Description);
			spdlog::info("  - Version: {}", Version.ToString());
		}
	};
}
