#pragma once

#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a content that is deserializable.
	class DeserializableContent
	{
	public:
		/// @brief Virtual destructor.
		virtual ~DeserializableContent();

		/// @brief Get the memory view of this content.
		///	@return The memory view of this content.
		[[nodiscard]] virtual MemoryView GetMemoryView() const = 0;

		/// @brief Get the size of the deserialization resource.
		///	@return The size of the deserialization resource.
		[[nodiscard]] virtual MemoryView::SizeType GetDeserializationResourceSize() const;

		/// @brief Deserialize data from the buffer.
		///	@param buffer Where the data is stored.
		///	@return Whether the deserialization is succeeded.
		virtual bool Deserialize(const MemoryView& buffer);
	};
}
