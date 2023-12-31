#pragma once

#include <ostream>

namespace LangYa::SentryLib
{
	/// @brief 表示可以转换为短且信息量大的字符串并输出到流中的对象。
	/// @details 因为短且信息量大的字符串一般对控制台输出很友好，所以就取了这个名字。
	/// 太长了 QAQ 。
	class CanStreamFormatToConsoleFriendlyString
	{
	public:
		/// @brief 接口的虚析构函数。
		virtual ~CanStreamFormatToConsoleFriendlyString();

		/// @brief 格式化并输出到流中。
		virtual std::ostream& FormatToConsoleFriendlyString(std::ostream& stream) const = 0;

		/// @brief 格式化为短且信息量大的字符串并输出到流中，调用如下函数：
		///	@code
		///	LangYa::SentryLib::CanStreamFormatToConsoleFriendlyString::FormatToConsoleFriendlyString 
		///	@endcode 
		/// @param stream 输出的流。
		///	@param obj 被格式化的对象。
		/// @return 参数中的流。
		friend std::ostream& operator<<(std::ostream& stream, const CanStreamFormatToConsoleFriendlyString& obj);

		/// @brief 调用 FormatToConsoleFriendlyString 格式化为短且信息量大的字符串并输出到流中。
		///	@details 如果 objPtr 为 nullptr，则不会调用 FormatToConsoleFriendlyString，将会调用 spdlog 打印内容。
		/// @param stream 输出的流。
		///	@param objPtr 被格式化的对象的指针。
		/// @return 参数中的流。
		friend std::ostream& operator<<(std::ostream& stream, const CanStreamFormatToConsoleFriendlyString* objPtr);
	};

	/// @brief 利用 std::stringstream 将 CanStreamFormatToConsoleFriendlyString 对象转化为字符串。
	///	这玩意儿性能损失很严重，当你需要转换很多对象时，使用类里面的流式转换。
	std::string FormatToConsoleFriendlyString(const CanStreamFormatToConsoleFriendlyString& obj);
}
