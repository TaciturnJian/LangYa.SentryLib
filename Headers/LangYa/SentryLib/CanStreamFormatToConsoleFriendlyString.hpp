#pragma once

#include <ostream>
#include <spdlog/spdlog.h>

namespace LangYa::SentryLib
{
	class CanStreamFormatToConsoleFriendlyString
	{
	public:
		virtual ~CanStreamFormatToConsoleFriendlyString();

		virtual std::ostream& FormatToConsoleFriendlyString(std::ostream& stream) const = 0;

		/// @brief 调用 FormatToConsoleFriendlyString 格式化为短且信息量大的字符串并输出到流中。
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

	std::string FormatToConsoleFriendlyString(const CanStreamFormatToConsoleFriendlyString& obj);
}
