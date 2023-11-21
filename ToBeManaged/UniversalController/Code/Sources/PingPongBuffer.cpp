#include <SangoC/Memory/Buffers/PingPongBuffer.hpp>

SangoC::Memory::Buffers::PingPongBuffer::PingPongBuffer(UniqueBuffer&& memory) :
	Ping(memory.GetView(), 0, memory.GetView().Size / 2),
	Pong(memory.GetView(), Ping.Size, Ping.Size),
	InnerBuffer(std::move(memory))
{
}

const SangoC::Memory::MemoryView& 
SangoC::Memory::Buffers::PingPongBuffer
::GetPing() const
{
	return Ping;
}

const SangoC::Memory::MemoryView& 
SangoC::Memory::Buffers::PingPongBuffer
::GetPong() const
{
	return Pong;
}

bool
SangoC::Memory::Buffers::PingPongBuffer
::Exchange(const MemoryView& resource, const MemoryView& destination) const noexcept
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	Ping.ReadFrom(resource);
	for (MemoryView::SizeType skipped_bytes = 0; skipped_bytes < Ping.Size; ++skipped_bytes)
	{
		if (Ping[skipped_bytes] != 33) continue;

		// ReSharper disable once CppExpressionWithoutSideEffects
		destination.ReadFrom(Ping.Head + skipped_bytes);
		Pong.ReadFrom(MemoryView{Ping, 0, skipped_bytes});
		return true;
	}

	return false;
}
