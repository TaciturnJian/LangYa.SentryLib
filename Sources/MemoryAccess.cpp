#include <LangYa/SentryLib/MemoryAccess.hpp>

LangYa::SentryLib::MemoryAccess
::MemoryAccess(MemoryView view) :
	View(std::move(view))
{
}

void
LangYa::SentryLib::MemoryAccess
::Connect()
{
}

bool LangYa::SentryLib::MemoryAccess
::IsConnected()
{
	return View.IsValid();
}

void LangYa::SentryLib::MemoryAccess
::Disconnect()
{
}

LangYa::SentryLib::MemoryView::SizeType LangYa::SentryLib::MemoryAccess
::Read(const MemoryView& view)
{
	View.CopyTo(view);
	return view.Size < View.Size ? view.Size : View.Size;
}

LangYa::SentryLib::MemoryView::SizeType LangYa::SentryLib
::MemoryAccess::Write(const MemoryView& view)
{
	View.ReadFrom(view);
	return view.Size < View.Size ? view.Size : View.Size;
}
