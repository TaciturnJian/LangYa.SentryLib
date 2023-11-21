#include <SangoC/Memory/Buffers/UniqueBuffer.hpp>

SangoC::Memory::Buffers::UniqueBuffer
::UniqueBuffer()
= default;

SangoC::Memory::Buffers::UniqueBuffer
::UniqueBuffer(const MemoryView::SizeType& size) :
	UniqueBytes(new MemoryView::ByteType[size]{}),
	View{UniqueBytes, UniqueBytes == nullptr ? 0 : size}
{
}

SangoC::Memory::Buffers::UniqueBuffer
::~UniqueBuffer()
{
	delete[] UniqueBytes;
}

SangoC::Memory::Buffers::UniqueBuffer
::UniqueBuffer(UniqueBuffer&& other) noexcept
{
	delete[] UniqueBytes;
	UniqueBytes = other.UniqueBytes;
	other.UniqueBytes = nullptr;
	View = other.View;
	other.View = MemoryView{};
}

SangoC::Memory::Buffers::UniqueBuffer&
SangoC::Memory::Buffers::UniqueBuffer
::operator=(UniqueBuffer&& other) noexcept
{
	if (this == &other) return *this;

	delete[] UniqueBytes;
	UniqueBytes = other.UniqueBytes;
	other.UniqueBytes = nullptr;
	View = other.View;
	other.View = MemoryView{};
	return *this;
}


const SangoC::Memory::MemoryView&
SangoC::Memory::Buffers::UniqueBuffer
::GetView() const
{
	return View;
}
