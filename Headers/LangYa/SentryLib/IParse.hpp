#pragma once

#include <istream>
#include <string_view>
#include <string>

namespace LangYa::SentryLib {
	/// @brief 代表可以从目标中解析出数据并存储于自身的对象。
	/// @tparam TParseTarget 被解析的目标。
	template<typename TParseTarget>
	class IParse {
	public:
		virtual ~IParse() = default;

		/// @brief 解析目标，将解析结果应用于自身。
		/// @param target 被解析的目标。
		/// @param option 解析设置。
		/// @return 是否完全成功解析。
		virtual bool Parse(const TParseTarget& target, int option = 0) = 0;
	};

	using IParseString = IParse<std::string>;

	class IParseStringView
	{
	public:
		virtual ~IParseStringView() = default;

		/// @brief 解析目标，将解析结果应用于自身。
		/// @param target 被解析的目标，注意，是否有更改取决于具体实现。
		/// @param option 解析设置。
		/// @return 是否完全成功解析。
		virtual bool Parse(std::string_view target, int option = 0) = 0;
	};

	class IParseStream
	{
	public:
		virtual ~IParseStream() = default;

		/// @brief 解析目标，将解析结果应用于自身。
		///	@param target 被解析的目标，注意，是否有更改取决于具体实现。
		///	@param option 解析设置。
		/// @return 是否完全成功解析。
		virtual bool Parse(std::istream& target, int option = 0) = 0;
	};
}