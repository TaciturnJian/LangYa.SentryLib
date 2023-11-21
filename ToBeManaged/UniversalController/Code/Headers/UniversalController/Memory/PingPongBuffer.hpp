#pragma once

#include <SangoC/Memory/MemoryView.hpp>
#include <SangoC/Memory/Buffers/UniqueBuffer.hpp>

namespace SangoC::Memory::Buffers
{
	class PingPongBuffer
	{
		MemoryView Ping{};
		MemoryView Pong{};
		UniqueBuffer InnerBuffer{};

 	public:
		explicit PingPongBuffer(UniqueBuffer&& memory);

		[[nodiscard]] const MemoryView& GetPing() const;

		[[nodiscard]] const MemoryView& GetPong() const;

		[[nodiscard]] bool Exchange(const MemoryView& resource, const MemoryView& destination) const noexcept;
	};
}
