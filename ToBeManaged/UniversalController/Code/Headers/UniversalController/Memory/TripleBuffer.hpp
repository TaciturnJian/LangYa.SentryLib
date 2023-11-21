#pragma once

#include <atomic>

#include <SangoC/Memory/Buffers/UniqueBuffer.hpp>
#include <SangoC/Memory/Resources/Resource.hpp>

namespace SangoC::Memory::Buffers
{
	class TripleBuffer
	{
		using ResourceType = Resources::Resource<MemoryView>;

		std::atomic_char WriterIndex{0};
		std::atomic_char ReaderIndex{0};

		UniqueBuffer InnerBuffer{};
		Resources::Resource<MemoryView> Resources[3]{};

	public:
		explicit TripleBuffer(MemoryView::SizeType size);

		bool Pop(const MemoryView& destination);

		void Push(const MemoryView& resource);
	};
}
