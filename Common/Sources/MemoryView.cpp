#include <cstring>

#include <spdlog/spdlog.h>

#include <LangYa/Common/MemoryView.hpp>

namespace LangYa::Common
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

	MemoryView
	::operator boost::asio::mutable_buffer() const
	{
		return boost::asio::buffer(Head, Size); 
	}

	MemoryView
	::operator std::string_view() const noexcept
	{
		return {reinterpret_cast<std::string_view::const_pointer>(Head), Size};
	}

	std::ostream&
	MemoryView
	::FormatByStream(std::ostream& stream, const int option) const
	{
		if (option == 0) // 控制台友好型流式输出十六进制数字，空格隔开（注意会更改流的设置），输出格式为 "xx xx xx xx"
		{
			stream << std::hex; // 设置流的输出为十六进制数字
			for (SizeType i = 0; i < Size - 1; ++i)
			{
				stream << static_cast<int>(Head[i]);
			}
			return stream << static_cast<int>(Head[Size - 1]);
		}

		if (option == 1) // json 紧凑格式输出，输出数组，输出格式为 "[xx,xx,xx,xx]"
		{
			stream << std::hex << '[';
			for (SizeType i = 0; i < Size - 1; ++i)
			{
				stream << static_cast<int>(Head[i]) << ',';
			}
			return stream << static_cast<int>(Head[Size - 1]) << ']';
		}

		if (option == 2) // json 宽松格式输出，输出数组，输出格式为 "[xx, xx, xx, xx]"
		{
			stream << std::hex << '[';
			for (SizeType i = 0; i < Size - 1; ++i)
			{
				stream << static_cast<int>(Head[i]) << ", ";
			}
			return stream << static_cast<int>(Head[Size - 1]) << ']';
		}

		spdlog::warn("MemoryView> Unknown option for formatting this view, trying with default option");

		return FormatByStream(stream);
	}
}
