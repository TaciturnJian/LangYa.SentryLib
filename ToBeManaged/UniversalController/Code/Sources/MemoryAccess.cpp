#include <SangoC/Communication/Connections/MemoryAccess.hpp>

SangoC::Communication::Connections::MemoryAccess
::MemoryAccess(Memory::MemoryView view) :
	View(std::move(view))
{
}

void
SangoC::Communication::Connections::MemoryAccess
::Connect()
{
}

bool SangoC::Communication::Connections::MemoryAccess
::IsConnected()
{
	return View.IsValid();
}

void SangoC::Communication::Connections::MemoryAccess
::Disconnect()
{
}

SangoC::Memory::MemoryView::SizeType SangoC::Communication::Connections::MemoryAccess
::Read(const Memory::MemoryView& view)
{
	return View.CopyTo(view);
}

SangoC::Memory::MemoryView::SizeType SangoC::Communication::Connections
::MemoryAccess::Write(const Memory::MemoryView& view)
{
	return View.ReadFrom(view);
}
