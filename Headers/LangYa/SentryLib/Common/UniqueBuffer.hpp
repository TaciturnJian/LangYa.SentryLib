#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Common/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a buffer with unique memory.
	class UniqueBuffer
	{
		/// @brief Hold the view of the unique memory.
		MemoryView View{};

	public:
		/// @brief Construct an empty buffer.
		UniqueBuffer();

		/// @brief Construct a new UniqueBuffer object with provided size.
		explicit UniqueBuffer(const MemoryView::SizeType& size);

		/// @brief Deconstruct, delete the unique memory in this buffer.
		~UniqueBuffer();

		/// @brief Deleted copy assignment operator.
		UniqueBuffer(const UniqueBuffer&) = delete;

		/// @brief Deleted copy assignment operator.
		UniqueBuffer& operator=(const UniqueBuffer&) = delete;

		/// @brief Basic move constructor, delete the unused memory here and move the new one.
		///	@param other The other buffer to be moved.
		UniqueBuffer(UniqueBuffer&& other) noexcept;

		/// @brief Basic move assignment operator, delete the unused memory here and move the new one.
		///	@param other The other buffer to be moved.
		///	@return The reference to this.
		UniqueBuffer& operator=(UniqueBuffer&& other) noexcept;

		/// @brief Get the view of the unique memory in the buffer.
		///	@return The view of the unique memory in the buffer.
		[[nodiscard]] const MemoryView& GetView() const;
	};
}
