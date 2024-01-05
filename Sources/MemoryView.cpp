#include <cstring>

#include <LangYa/SentryLib/MemoryView.hpp>

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
	::~MemoryView()
	{
		Head = nullptr;
		Size = 0;
	}

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
		if (!IsValid()) return;

		if (Head == head || head == nullptr) return;

		memcpy(Head, head, Size);
	}

	void
	MemoryView
	::ReadFrom(const MemoryView& view) const
	{
		if (!IsValid()) return;

		if (Head == view.Head || view.Head == nullptr) return;

		memcpy(Head, view.Head, view.Size < Size ? view.Size : Size);
	}

	void
	MemoryView
	::CopyTo(void* const head) const
	{
		if (!IsValid()) return;

		if (Head == head || head == nullptr) return;

		memcpy(head, Head, Size);
	}

	void
	MemoryView
	::CopyTo(const MemoryView& view) const
	{
		if (!IsValid()) return;

		if (Head == view.Head || view.Head == nullptr) return;

		memcpy(view.Head, Head, view.Size < Size ? view.Size : Size);
	}

	void
	MemoryView::SetValue(const ByteType byte) const
	{
		if (!IsValid()) return;

		memset(Head, byte, Size);
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

	std::ostream& 
	operator<<(std::ostream& stream, const MemoryView& view)
	{
		for (MemoryView::SizeType i = 0; i < view.Size - 1; i++)
		{
			stream << std::hex << static_cast<int>(view[i]) << ' ';
		}

		return stream << std::hex << static_cast<int>(view[view.Size - 1]);
	}
}
