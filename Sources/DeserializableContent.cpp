#include <LangYa/SentryLib/DeserializableContent.hpp>

namespace LangYa::SentryLib
{
	DeserializableContent
	::~DeserializableContent()
	= default;

	MemoryView::SizeType
	DeserializableContent
	::GetDeserializationResourceSize() const
	{
		return GetDeserializationMemoryView().Size;
	}

	bool
	DeserializableContent
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
}
