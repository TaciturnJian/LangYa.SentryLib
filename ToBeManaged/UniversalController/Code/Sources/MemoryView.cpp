#include <SangoC/Memory/MemoryView.hpp>

#include <cstring>

SangoC::Memory::MemoryView
::MemoryView()
= default;

SangoC::Memory::MemoryView
::MemoryView(const MemoryView& view, const SizeType& offset, const SizeType& size) :
	Head(view.Head + offset),
	Size(size == 0 ? view.Size - offset : size)
{
}

SangoC::Memory::MemoryView::MemoryView(const MemoryView& other)
= default;

SangoC::Memory::MemoryView::MemoryView(MemoryView&& other) noexcept
= default;

SangoC::Memory::MemoryView& SangoC::Memory::MemoryView::operator=(const MemoryView& other)
= default;

SangoC::Memory::MemoryView& SangoC::Memory::MemoryView::operator=(MemoryView&& other) noexcept
= default;

SangoC::Memory::MemoryView::ByteType& 
SangoC::Memory::MemoryView
::operator[](const SizeType& index) const
{
	return Head[index];
}

SangoC::Memory::MemoryView::SizeType
SangoC::Memory::MemoryView
::ReadFrom(const void* const head) const
{
	if (Head == head) return Size;

	memcpy(Head, head, Size);
	return Size;
}

SangoC::Memory::MemoryView::SizeType
SangoC::Memory::MemoryView
::ReadFrom(const MemoryView& view) const
{
	if (Head == view.Head) return Size;

	const auto& size = view.Size < Size ? view.Size : Size;
	memcpy(Head, view.Head, size);
	return size;
}

SangoC::Memory::MemoryView::SizeType
SangoC::Memory::MemoryView
::CopyTo(void* const head) const
{
	if (Head == head) return Size;

	memcpy(head, Head, Size);
	return Size;
}

SangoC::Memory::MemoryView::SizeType
SangoC::Memory::MemoryView
::CopyTo(const MemoryView& view) const
{
	if (Head == view.Head) return Size;

	const auto& size = view.Size < Size ? view.Size : Size;
	memcpy(view.Head, Head, size);
	return size;
}

SangoC::Memory::MemoryView
::~MemoryView()
{
	Head = nullptr;
	Size = 0;
}

bool
SangoC::Memory::MemoryView
::IsValid() const noexcept
{
	return Head != nullptr && Size > 0;
}

boost::asio::mutable_buffer
SangoC::Memory::MemoryView
::ToBuffer() const
{
	return boost::asio::buffer(Head, Size);
}
