#pragma once

#include <LangYa/Common/MemoryView.hpp>

namespace LangYa::Common {
	class ISerializable 
	{
	public:
		using SizeType = MemoryView::SizeType;

		virtual ~ISerializable() = default;
		virtual SizeType GetSerializationResultSize() = 0;
		virtual bool Serialize(const MemoryView& buffer) = 0;
		virtual bool Deserialize(const MemoryView& buffer) = 0;
	};
}