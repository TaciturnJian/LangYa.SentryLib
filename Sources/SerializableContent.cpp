#include <LangYa/SentryLib/SerializableContent.hpp>

LangYa::SentryLib::SerializableContent
::~SerializableContent()
= default;

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::SerializableContent
::GetSerializationResultSize() const
{
	return GetSerializationMemoryView().Size;
}

bool
LangYa::SentryLib::SerializableContent
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
