#pragma once

#include <string>
#include <ostream>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/SerialNumbers.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一些简单的应用程序信息，包装了简单的信息输出方法。
	struct ApplicationInfo final
	{
		/// @brief 应用程序的名称。
		std::string Name{"ApplicationName"};

		/// @brief 应用程序的简述。
		std::string Description{"ApplicationDescription"};

		/// @brief 应用程序的版本。
		SerialNumbers<unsigned short> Version{1, 0, 0, 0};

		[[nodiscard]] std::string ToString() const;

		friend std::ostream& operator<<(std::ostream& stream, const ApplicationInfo& info);

		// ReSharper disable once CppInconsistentNaming
		void OutputTo_spdlog() const;
	};
}
