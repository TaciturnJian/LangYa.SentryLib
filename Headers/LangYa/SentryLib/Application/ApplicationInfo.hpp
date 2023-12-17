#pragma once

#include <string>

#include <LangYa/SentryLib/Common/SerialNumbers.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一些简单的应用程序信息，包装了简单的信息输出方法。
	///	使用 stream , spdlog 和 fmt 等将保存的信息格式化输出到控制台。
	///	在使程序代码更美观的同时，方便开发者在调试时（日志中）查看程序的一些基本信息。
	///	@code
	///	ApplicationInfo
	///	{
	///		"ExampleApp",
	///		"An application with sentry lib!",
	///		{1, 0, 0, 0}
	///	}.OutputTo_spdlog();
	///	@endcode
	struct ApplicationInfo final
	{
		/// @brief 应用程序的名称。
		///	一般在主线程的日志输出中会打印类似这种格式的内容： "Name> Message"。
		///	使用此种格式可以方便地区分应用程序不同模块的日志输出。
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
		///	此种 json 格式可以简单地转化成 C# 中的 JObject 对象。
		///	@param stream 格式化的结果输出到此流中。
		///	@param info 应用程序信息。
		///	@return 参数中的流。
		friend std::ostream& operator<<(std::ostream& stream, const ApplicationInfo& info);

		/// @brief 使用 spdlog 输出。
		///	输出的内容不再是 json 格式，并且有一定的缩进。
		///	如果使用了spdlog配置多个日志记录器，还可以在日志中确定内容。
		// ReSharper disable once CppInconsistentNaming
		void OutputTo_spdlog() const;
	};
}
