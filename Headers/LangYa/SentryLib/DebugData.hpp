#pragma once

#include <string>
#include <cstdint>

namespace LangYa::SentryLib
{
	/// @brief 利用 spdlog 打印生存周期信息的类。
	///	@details 会在构建、析构时打印信息。
	struct DebugData
	{
		/// @brief 数字ID，用于区分不同的实例，打印内容在日志中的格式为 [Name:ID]。
		std::uint64_t ID{0};

		/// @brief 字符串ID，用于自己区分不同的实例，打印内容在日志中的格式为 [Name:ID]。
		std::string Name{"<DebugDataName>"};

		DebugData();
		explicit DebugData(std::string name);
		~DebugData();
	};
}
