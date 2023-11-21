#pragma once

#include <SangoC/Memory/MemoryView.hpp>

namespace SangoC::Memory::Buffers
{
	class UniqueBuffer
	{
		MemoryView::ByteType *UniqueBytes{nullptr};
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
