#pragma once

#include <ostream>
#include <string>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <LangYa/SentryLib/SerialNumbers.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一些简单的应用程序信息，包装了简单的信息输出方法。
	///	主要利用到 stream spdlog 和 fmt 等将此类的信息格式化输出到控制台。
	struct ApplicationInfo final
	{
		/// @brief 应用程序的名称。
		std::string Name{"ApplicationName"};

		/// @brief 应用程序的简述。
		std::string Description{"ApplicationDescription"};

		/// @brief 应用程序的版本。
		SerialNumbers<unsigned short> Version{1, 0, 0, 0};

		/// @brief 转为字符串。
		[[nodiscard]] std::string ToString() const;

		/// @brief 向流中输入内容。
		friend std::ostream& operator<<(std::ostream& stream, const ApplicationInfo& info);

		/// @beif 使用 spdlog 输出。
		// ReSharper disable once CppInconsistentNaming
		void OutputTo_spdlog() const;
	};
}
