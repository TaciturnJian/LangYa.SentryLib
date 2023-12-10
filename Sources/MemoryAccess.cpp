#include <LangYa/SentryLib/MemoryAccess.hpp>

namespace LangYa::SentryLib
{
	MemoryAccess
	::MemoryAccess(const MemoryView& view) :
		View(view)
	{
	}

	void
	MemoryAccess
	::Connect()
	{
	}

	bool MemoryAccess
	::IsConnected()
	{
		return View.IsValid();
	}

	void MemoryAccess
	::Disconnect()
	{
	}

	MemoryView::SizeType
	MemoryAccess
	::Read(const MemoryView& view)
	{
		View.CopyTo(view);
		return view.Size < View.Size ? view.Size : View.Size;
	}

	MemoryView::SizeType
	MemoryAccess::Write(const MemoryView& view)
	{
		View.ReadFrom(view);
		return view.Size < View.Size ? view.Size : View.Size;
	}
}
