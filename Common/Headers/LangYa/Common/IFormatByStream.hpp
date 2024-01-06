#pragma once

#include <ostream>
#include <string>

namespace LangYa::Common
{
	/// @brief 代表一种可以通过流格式化的对象。
	class IFormatByStream
	{
	public:
		virtual ~IFormatByStream();

		/// @brief 将对象格式化到流中。
		///	@param stream 对象格式化时需要的流，例如 std::cout 或者 std::stringstream 。
		///	@param option 格式化的设置，请参阅具体实现。
		///	@warning 传入的流在被调用时状态往往会被改变，例如使用了 std::hex 改变进制等，如果有严格要求，请仔细阅读实现者的文档。
		///	@return 参数中的 stream 。
		virtual std::ostream& FormatByStream(std::ostream& stream, int option = 0) const = 0;

		/// @brief 格式化此对象到流中，令 obj 调用如下函数，其选项是默认值 0 。
		/// LangYa::SentryLib::FormatByStream 。
		friend std::ostream& operator<<(std::ostream& stream, const IFormatByStream& obj);
	};

	/// @brief 利用 std::stringstream 将 IFormatByStream 对象输入到流中，并从流中读取字符串。
	///	@details 构建 std::stringstream ，调用 IFormatByStream::FormatByStream 方法，再利用 std::stringstream::str 取出字符串。
	///	有性能损失，如果需要频繁调用，建议自行实现。
	std::string FormatToString(const IFormatByStream& obj, int option = 0);
}
