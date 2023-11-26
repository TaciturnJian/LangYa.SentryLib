#pragma once

#include <atomic>

#include <LangYa/SentryLib/UniqueBuffer.hpp>
#include <LangYa/SentryLib/Resource.hpp>

namespace LangYa::SentryLib
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
