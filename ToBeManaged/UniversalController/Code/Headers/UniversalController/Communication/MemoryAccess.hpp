#pragma once

#include <SangoC/Memory/MemoryView.hpp>
#include <SangoC/Communication/Connections/Connection.hpp>

namespace SangoC::Communication::Connections
{
	class MemoryAccess final : public Connection
	{
		Memory::MemoryView View;

	public:
		explicit MemoryAccess(Memory::MemoryView view);

		void Connect() override;
		bool IsConnected() override;
		void Disconnect() override;

		Memory::MemoryView::SizeType Read(const Memory::MemoryView& view) override;

		Memory::MemoryView::SizeType Write(const Memory::MemoryView& view) override;
	};

}
