#pragma once

#include <string>

#include <LangYa/SentryLib/SerialNumbers.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一些简单的应用程序信息，包装了简单的信息输出方法。
	///	主要利用到 stream spdlog 和 fmt 等将此类的信息格式化输出到控制台。
	struct ApplicationInfo final
	{
		/// @brief 应用程序的名称。
		///	一般在主线程的日志输出中会打印类似这种格式的内容： "Name> Message"。
		///	使用此种格式可以方便的区分应用程序不同模块的日志输出。
		std::string Name{"ApplicationName"};

		/// @brief 应用程序的简述。
		///	在程序运行时会打印的简短描述。
		std::string Description{"ApplicationDescription"};

		/// @brief 应用程序的版本。
		///	在程序运行时会打印的版本号，便于开发者确定问题是否在新的版本中存在。
		SerialNumbers<unsigned short> Version{1, 0, 0, 0};

		/// @brief 转为 json 字符串。
		///	调用了流式输出，见 @code operator<<(std::ostream& stream, const ApplicationInfo& info) @endcode
		[[nodiscard]] std::string ToString() const;

		/// @brief 向流中输入内容。
		///	输出格式为{"Name":"<Name>","Description":"<Description>","Version":"<Version>"}。
		///	@param stream 格式化的结果输出到此流中。
		///	@param info 应用程序信息。
		///	@return 参数中的流。
		friend std::ostream& operator<<(std::ostream& stream, const ApplicationInfo& info);

		/// @brief 使用 spdlog 输出。
		///	输出的内容不再是 json 格式，并且有一定的缩进。
		// ReSharper disable once CppInconsistentNaming
		void OutputTo_spdlog() const;
	};
}
