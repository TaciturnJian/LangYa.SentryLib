#pragma once

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a buffer with unique memory.
	class UniqueBuffer
	{
		MemoryView View{};

	public:
		UniqueBuffer();

		explicit UniqueBuffer(const MemoryView::SizeType& size);

		~UniqueBuffer();

		UniqueBuffer(const UniqueBuffer& other) = delete;

		UniqueBuffer& operator=(const UniqueBuffer& other) = delete;

		UniqueBuffer(UniqueBuffer&& other) noexcept;

		UniqueBuffer& operator=(UniqueBuffer&& other) noexcept;

		[[nodiscard]] const MemoryView& GetView() const;
	};
}
