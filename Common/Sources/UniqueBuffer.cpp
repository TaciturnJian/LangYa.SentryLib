#include <spdlog/spdlog.h>

#include <LangYa/Common/UniqueBuffer.hpp>

namespace LangYa::Common
{
	UniqueBuffer
	::UniqueBuffer()
	= default;

	UniqueBuffer
	::UniqueBuffer(const MemoryView::SizeType& size) :
		View(new MemoryView::ByteType[size]{}, size)
	{
		if (size > 0 && View.Head == nullptr)
		{
			View.Size = 0;
			spdlog::warn("UniqueBuffer> Tried acquire {} bytes, but failed. Maybe run out of memory", size);
		}
	}

	UniqueBuffer
	::~UniqueBuffer()
	{
		delete[] View.Head;
	}

	UniqueBuffer
	::UniqueBuffer(UniqueBuffer&& other) noexcept
	{
		delete[] View.Head;
		View = other.View;
		other.View = MemoryView{};
	}

	UniqueBuffer&
	UniqueBuffer
	::operator=(UniqueBuffer&& other) noexcept
	{
		if (this == &other) return *this;

		delete[] View.Head;
		View = other.View;
		other.View = MemoryView{};
		return *this;
	}


	const MemoryView&
	UniqueBuffer
	::GetView() const
	{
		return View;
	}
}
