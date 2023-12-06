#include <LangYa/SentryLib/MemoryView.hpp>

#include <cstring>

namespace LangYa::SentryLib
{
	MemoryView
	::MemoryView() noexcept
	= default;

	MemoryView
	::MemoryView(const MemoryView& view, const SizeType& offset, const SizeType& size) :
		Head(view.Head + offset),
		Size(size == 0 ? view.Size - offset : size)
	{
	}

	MemoryView
	::MemoryView(const MemoryView& other)
	= default;

	MemoryView
	::MemoryView(MemoryView&& other) noexcept
	= default;

	MemoryView&
	MemoryView
	::operator=(const MemoryView& other)
	= default;

	MemoryView&
	MemoryView
	::operator=(MemoryView&& other) noexcept
	= default;

	MemoryView::ByteType&
	MemoryView
	::operator[](const SizeType& index) const
	{
		return Head[index];
	}

	void
	MemoryView
	::ReadFrom(const void* const head) const
	{
		if (Head == head) return;
		memcpy(Head, head, Size);
	}

	void
	MemoryView
	::ReadFrom(const MemoryView& view) const
	{
		if (Head == view.Head) return;
		memcpy(Head, view.Head, view.Size < Size ? view.Size : Size);
	}

	void
	MemoryView
	::CopyTo(void* const head) const
	{
		if (Head == head) return;
		memcpy(head, Head, Size);
	}

	void
	MemoryView
	::CopyTo(const MemoryView& view) const
	{
		if (Head == view.Head) return;
		memcpy(view.Head, Head, view.Size < Size ? view.Size : Size);
	}

	MemoryView
	::~MemoryView()
	{
		Head = nullptr;
		Size = 0;
	}

	bool
	MemoryView
	::IsValid() const noexcept
	{
		return Head != nullptr && Size > 0;
	}

	boost::asio::mutable_buffer
	MemoryView
	::ToBuffer() const
	{
		return boost::asio::buffer(Head, Size);
	}
}
