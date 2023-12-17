#pragma once

#include <LangYa/SentryLib/Common/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一个可被序列化的东西。
	///	可被序列化即信息可以无损地转换为二进制数据。
	///	一般将内容序列化后，将其写入文件或者网络传输，可以减轻读写传输等IO操作的压力。
	class SerializableContent
	{
	public:
		/// @brief 虚拟的析构器，防止有笨蛋把指针强制转换为此类后错误解析了内容。
		virtual ~SerializableContent();

		/// @brief 获取序列化结果的大小。
		///	返回序列化结果的大小，如果值为0，那么这个东西不可以被序列化。（谁继承这个接口然后返回0啊？）
		///	建议遇到这种情况时直接抛出异常。
		///	@return 序列化结果的大小。
		[[nodiscard]] virtual MemoryView::SizeType GetSerializationResultSize() const = 0;

		/// @brief 序列化此内容，将序列化的结果写入缓冲区。
		///	当提供的缓冲区太小时，不会执行序列化操作，返回false。
		///	@param buffer 保存序列化结果的缓冲区。
		///	@return 操作是否全部成功。
		[[nodiscard]] virtual bool Serialize(const MemoryView& buffer) = 0;
	};

	/// @brief 代表一个可以序列化的类型，即可以被转换为 @code LangYa::SentryLib::SerializableContent @endcode 的类型。
	///	@tparam T 概念中被检查的类型
	template <typename T>
	concept Serializable = requires(
		SerializableContent* base,
		T* p,
		bool result
	)
		{
			base = p;
		};
}
