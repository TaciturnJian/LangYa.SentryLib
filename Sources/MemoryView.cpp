#include <LangYa/SentryLib/MemoryView.hpp>

#include <cstring>

LangYa::SentryLib::MemoryView
::MemoryView() noexcept
= default;

LangYa::SentryLib::MemoryView
::MemoryView(const MemoryView& view, const SizeType& offset, const SizeType& size) :
	Head(view.Head + offset),
	Size(size == 0 ? view.Size - offset : size)
{
}

LangYa::SentryLib::MemoryView::MemoryView(const MemoryView& other)
= default;

LangYa::SentryLib::MemoryView::MemoryView(MemoryView&& other) noexcept
= default;

LangYa::SentryLib::MemoryView& LangYa::SentryLib::MemoryView::operator=(const MemoryView& other)
= default;

LangYa::SentryLib::MemoryView& LangYa::SentryLib::MemoryView::operator=(MemoryView&& other) noexcept
= default;

LangYa::SentryLib::MemoryView::ByteType& 
LangYa::SentryLib::MemoryView
::operator[](const SizeType& index) const
{
	return Head[index];
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::MemoryView
::ReadFrom(const void* const head) const
{
	if (Head == head) return Size;

	memcpy(Head, head, Size);
	return Size;
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::MemoryView
::ReadFrom(const MemoryView& view) const
{
	if (Head == view.Head) return Size;

	const auto& size = view.Size < Size ? view.Size : Size;
	memcpy(Head, view.Head, size);
	return size;
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::MemoryView
::CopyTo(void* const head) const
{
	if (Head == head) return Size;

	memcpy(head, Head, Size);
	return Size;
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::MemoryView
::CopyTo(const MemoryView& view) const
{
	if (Head == view.Head) return Size;

	const auto& size = view.Size < Size ? view.Size : Size;
	memcpy(view.Head, Head, size);
	return size;
}

LangYa::SentryLib::MemoryView
::~MemoryView()
{
	Head = nullptr;
	Size = 0;
}

bool
LangYa::SentryLib::MemoryView
::IsValid() const noexcept
{
	return Head != nullptr && Size > 0;
}

boost::asio::mutable_buffer
LangYa::SentryLib::MemoryView
::ToBuffer() const
{
	return boost::asio::buffer(Head, Size);
}
