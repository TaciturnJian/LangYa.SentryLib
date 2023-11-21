#pragma once

#include <cstring>

#include <boost/asio/buffer.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a view of memory.
	/// Record the head and the length of the memory.
	/// There is no guarantee that the memory is readable or writable.
	/// Every bytes of memory is based on trust.
	struct MemoryView
	{
		/// @brief Represent the type of "Size".
		using SizeType = unsigned long long;

		/// @brief Represent the type of "Byte".
		using ByteType = unsigned char;

		/// @brief Represent the type of memory "Head".
		using HeadType = ByteType*;

		/// @brief Record the head of the memory, basically the start of the sequence.
		///	If it's nullptr, don't try to use this memory view! It's invalid now!
		HeadType Head{nullptr};

		/// @brief Record the size of the memory, basically the length of the sequence.
		///	If it's zero, don't try to use this memory view! It's invalid now!
		SizeType Size{0};

		/// @brief Construct an invalid memory view with nullptr and zero.
		MemoryView() noexcept;

		/// @brief Construct a memory view with the given head and size.
		///	@param head The head of the array, it's type will be inferred automatically.
		///	@param count The element count of the array.
		template <typename THead>
		MemoryView(THead* const head, const SizeType& count) :
			// Who the hell knows what cast should I use!
			// ReSharper disable once CppCStyleCast
			Head((ByteType*)head),
			Size(count * sizeof(THead))
		{
		}

		/// @brief Construct a memory view by constructed view with offset and new size.
		///	@param view The constructed view.
		///	@param offset The offset of the head, only positive numbers are allowed.
		///	@param size The size of the new view, if it's zero, the size will be the rest of the view.
		MemoryView(const MemoryView& view, const SizeType& offset, const SizeType& size = 0);

		/// @brief Default copy constructor.
		///	@param other The copied one.
		MemoryView(const MemoryView& other);

		/// @brief Default move constructor.
		///	@param other The moved one.
		MemoryView(MemoryView&& other) noexcept;

		/// @brief Default copy constructor.
		///	@param other The copied one.
		///	@return The reference of this.
		MemoryView& operator=(const MemoryView& other);

		/// @brief Default move constructor.
		///	@param other The moved one.
		///	@return The reference of this.
		MemoryView& operator=(MemoryView&& other) noexcept;

		/// @brief Access memory like byte array.
		///	@param index The index of the byte.
		///	@return The reference of the byte.
		ByteType& operator[](const SizeType& index) const;

		/// @brief Read the given memory started with the head, fill all memory for this memory view, basically a wrap for memcpy.
		///	@param head The head of the memory.
		///	@return The size of transferred bytes.
		void ReadFrom(const void* head) const;

		/// @brief Read the given view. The target byte count is the min value of this->Size and view.Size.
		///	@param view The view to read.
		///	@return The size of transferred bytes.
		void ReadFrom(const MemoryView& view) const;

		/// @brief Write bytes to the given memory started with the head, fill all memory for this memory view, basically a wrap for memcpy.
		///	@param head The head of the memory.
		///	@return The size of transferred bytes.
		void CopyTo(void* head) const;

		/// @brief Write bytes to the given view. The target byte count is the min value of this->Size and view.Size.
		///	@param view The view to write to.
		///	@return The size of transferred bytes.
		void CopyTo(const MemoryView& view) const;

		/// @brief Destruct the memory view.
		///	Set the head to nullptr and set the size to zero.
		~MemoryView();

		/// @brief Check if the memory view is valid, basically check the head and the size.
		///	@warning The result true cannot guarantee the memory is valid.
		///	@return true if the memory view is valid, otherwise false.
		[[nodiscard]] bool IsValid() const noexcept;

		/// @brief Convert this view to boost::asio::mutable_buffer.
		///	@return A instance of boost::asio::mutable_buffer.
		[[nodiscard]] boost::asio::mutable_buffer ToBuffer() const;
	};
}
