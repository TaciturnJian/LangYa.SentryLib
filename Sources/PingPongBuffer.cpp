#include <LangYa/SentryLib/PingPongBuffer.hpp>

#include <cstring>

namespace LangYa::SentryLib
{
	PingPongBuffer
	::PingPongBuffer(MemoryView::SizeType size)
	{
	}

	PingPongBuffer
	::PingPongBuffer(UniqueBuffer&& memory) :
		Ping(memory.GetView(), 0, memory.GetView().Size / 2),
		Pong(memory.GetView(), Ping.Size, Ping.Size),
		InnerBuffer(std::move(memory))
	{
	}

	PingPongBuffer
	::PingPongBuffer(PingPongBuffer&& other) noexcept
	= default;

	PingPongBuffer& PingPongBuffer
	::operator=(PingPongBuffer&& other) noexcept
	= default;

	const MemoryView&
	PingPongBuffer
	::GetPing() const
	{
		return Ping;
	}

	const MemoryView&
	PingPongBuffer
	::GetPong() const
	{
		return Pong;
	}

	bool
	PingPongBuffer
	::Exchange(const MemoryView& resource, const MemoryView& destination) const
	{
		// If the destination is suitable, resource has more than 0 bytes and the head of resource is '!',
		// copy the memory immediately without this buffer.
		if (resource.Size == destination.Size && resource.Size == Ping.Size && resource.Size > 0 && resource[0] == '!')
		{
			destination.ReadFrom(resource.Head);
			return true;
		}

		Pong.ReadFrom(resource);
		for (MemoryView::SizeType skipped_bytes = 0; skipped_bytes <= Ping.Size; ++skipped_bytes)
		{
			if (Ping[skipped_bytes] != 33) continue;

			destination.ReadFrom(Ping.Head + skipped_bytes);
			memset(Ping.Head + skipped_bytes, 0, Ping.Size);
			Ping.ReadFrom(Pong);
			return true;
		}

		return false;
	}
	  
	const MemoryView& 
	PingPongBuffer
	::GetExchangeBuffer() const noexcept
	{
		return Pong;
	}

	bool
	PingPongBuffer
	::ExchangeWithExchangeBuffer(const MemoryView& destination) const
	{
		for (MemoryView::SizeType skipped_bytes = 0; skipped_bytes <= Ping.Size; ++skipped_bytes)
		{
			if (Ping[skipped_bytes] != 33) continue;

			destination.ReadFrom(Ping.Head + skipped_bytes);
			memset(Ping.Head + skipped_bytes, 0, Ping.Size);
			Ping.ReadFrom(Pong);
			return true;
		}

		return false;
	}
}
