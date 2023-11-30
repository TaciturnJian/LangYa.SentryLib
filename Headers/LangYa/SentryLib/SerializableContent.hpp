#pragma once
#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a content that is serializable.
	class SerializableContent
	{
	public:
		/// @brief Virtual destructor.
		virtual ~SerializableContent();

		/// @brief Get the memory view of this content.
		///	@return The memory view of this content.
		[[nodiscard]] virtual MemoryView GetMemoryView() const = 0;

		/// @brief Get the size of the serialization result.
		///	@return The size of the serialization result.
		[[nodiscard]] virtual MemoryView::SizeType GetSerializationResultSize() const;

		/// @brief Serialize data and write the result to buffer.
		///	@param buffer Where to write the result.
		///	@return Whether the serialization is succeeded.
		[[nodiscard]] virtual bool Serialize(const MemoryView& buffer);
	};

	/// @brief Check if the type T can be transformed into @code LangYa::SentryLib::SerializableContent @endcode.
	///	@tparam T The type to check.
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
