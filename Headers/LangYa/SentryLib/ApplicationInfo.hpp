#pragma once

#include <string>
#include <ostream>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/SerialNumbers.hpp>

namespace LangYa::SentryLib
{
	struct ApplicationInfo final
	{
		std::string Name{"ApplicationName"};
		std::string Description{"ApplicationDescription"};
		SerialNumbers<unsigned short> Version{1, 0, 0, 0};

		[[nodiscard]] std::string ToString() const;

		friend std::ostream& operator<<(std::ostream& stream, const ApplicationInfo& info);

		// ReSharper disable once CppInconsistentNaming
		void OutputTo_spdlog() const;
	};
}
