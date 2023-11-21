#pragma once

#include <SangoC/Memory/MemoryView.hpp>

namespace SangoC::Memory
{
	template<typename TResource, MemoryView::SizeType ResultSize>
	class Serializer
	{
	public:
		inline static MemoryView::SizeType CompressedResourceSize{ResultSize};

		static bool Serialize(const TResource& resource, const MemoryView& buffer)
		{
			return false;
		}

		static bool Deserialize(const MemoryView& buffer, TResource& resource)
		{
			return false;
		}
	};

}