#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <ostream>
#include <string>

namespace LangYa::SentryLib {

	/// @brief 表示此类可以格式化为 json 字符串并输出到流中。
	///	@warning 请不要在继承了此类后再声明下面的函数：
	///	@code friend std::ostream& operator<<(std::ostream& stream, const DerivedClass& instance) @endcode
	class CanStreamFormatToJsonString 
	{
	public:
		/// @brief 默认析构函数，用来正确析构某些内容。
		virtual ~CanStreamFormatToJsonString();

		/// @brief 格式化为 json 字符串并输出到流中。
		/// @param stream 输出的流。
		/// @return 参数中的流。
		virtual std::ostream& FormatToJsonString(std::ostream& stream) const = 0;

		/// @brief 调用 FormatToJsonString 格式化为 json 字符串并输出到流中。
		/// @param stream 输出的流。
		///	@param obj 被格式化的对象。
		/// @return 参数中的流。
		friend std::ostream& operator<<(std::ostream& stream, const CanStreamFormatToJsonString& obj);

		/// @brief 调用 FormatToJsonString 格式化为 json 字符串并输出到流中。
		///	@details 如果 objPtr 为 nullptr，则不会调用 FormatToJsonString，将会调用 spdlog 打印内容。
		/// @param stream 输出的流。
		///	@param objPtr 被格式化的对象的指针。
		/// @return 参数中的流。
		friend std::ostream& operator<<(std::ostream& stream, const CanStreamFormatToJsonString* objPtr);
	};

	/// @brief 利用 std::stringstream 将 CanStreamFormatToJsonString 对象转化为字符串。
	///	这玩意儿性能损失很严重，当你需要转换很多对象时，使用类里面的流式转换。
	[[nodiscard]] std::string FormatToJsonString(const CanStreamFormatToJsonString& obj);
}
