#include <LangYa/SentryLib/PingPongBuffer.hpp>

namespace LangYa::SentryLib
{
	PingPongBuffer
	::PingPongBuffer(UniqueBuffer&& memory) :
		Ping(memory.GetView(), 0, memory.GetView().Size / 2),
		Pong(memory.GetView(), Ping.Size, Ping.Size),
		InnerBuffer(std::move(memory))
	{
	}

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
	::Exchange(const MemoryView& resource, const MemoryView& destination) const noexcept
	{
		Pong.ReadFrom(resource);
		for (MemoryView::SizeType skipped_bytes = 0; skipped_bytes <= Ping.Size; ++skipped_bytes)
		{
			if (Ping[skipped_bytes] != 33) continue;

			destination.ReadFrom(Ping.Head + skipped_bytes);
			memset(Ping.Head + skipped_bytes, 0, Ping.Size - skipped_bytes);
			Ping.ReadFrom(Pong);
			return true;
		}

		return false;
	}
}
