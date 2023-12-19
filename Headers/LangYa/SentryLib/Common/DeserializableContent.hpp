#pragma once

#include <LangYa/SentryLib/Common/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一种可以反序列化的内容。
	///	此接口提供了反序列时所需要的两个函数： 获取反序列化资源的最小大小，以及从缓冲区中反序列化资源。
	class DeserializableContent
	{
	public:
		/// @brief 默认虚析构函数。
		virtual ~DeserializableContent();

		/// @brief 获取此内容的反序列化资源大小，大多数时候就是此内容的内存视图大小。
		///	@return 反序列化资源大小。
		[[nodiscard]] virtual MemoryView::SizeType GetDeserializationResourceSize() const = 0;

		/// @brief 从缓冲区中反序列化资源。
		///	@param buffer 反序列化资源数据的缓冲区。
		///	@return 是否反序列化成功。
		[[nodiscard]] virtual bool Deserialize(const MemoryView& buffer) = 0;
	};

	/// @brief 基本的可以反序列化的资源概念
	/// 即继承了： @code LangYa::SentryLib::DeserializableContent @endcode。
	///	@tparam T 受检查的类型。
	template <typename T>
	concept Deserializable = requires
	(DeserializableContent* base, T* p){ base = p; };
}
