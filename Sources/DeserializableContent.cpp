#include <LangYa/SentryLib/DeserializableContent.hpp>

LangYa::SentryLib::DeserializableContent
::~DeserializableContent()
= default;

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::DeserializableContent
::GetDeserializationResourceSize() const
{
	return GetDeserializationMemoryView().Size;
}

bool
LangYa::SentryLib::DeserializableContent
::Deserialize(const MemoryView& buffer)
{
	const auto view = GetDeserializationMemoryView();
	if (buffer.Size > view.Size)
	{
		return false;
	}

	view.ReadFrom(buffer);
	return true;
}
