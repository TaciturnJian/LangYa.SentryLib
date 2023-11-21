#pragma once

#include <string>

#include <SangoC/Memory/MemoryView.hpp>

namespace SangoC::Communication::Connections
{
	class Connection
	{
	public:
		virtual ~Connection() = default;
		virtual void Connect() = 0;
		[[nodiscard]] virtual bool IsConnected() = 0;
		virtual void Disconnect() = 0;
		virtual Memory::MemoryView::SizeType Read(const Memory::MemoryView& view) = 0;
		virtual Memory::MemoryView::SizeType Write(const Memory::MemoryView& view) = 0;
		virtual std::string ToString() { return "Connection"; }
	};
}

