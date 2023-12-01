#include <LangYa/SentryLib/SerializableContent.hpp>

namespace LangYa::SentryLib
{
	SerializableContent
	::~SerializableContent()
	= default;

	MemoryView::SizeType
	SerializableContent
	::GetSerializationResultSize() const
	{
		return GetSerializationMemoryView().Size;
	}

	bool
	SerializableContent
	::Serialize(const MemoryView& buffer)
	{
		const auto view = GetSerializationMemoryView();
		if (view.Size < buffer.Size)
		{
			return false;
		}

		view.CopyTo(buffer);
		return true;
	}
}
