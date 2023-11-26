#pragma once

#include <atomic>

#include <LangYa/SentryLib/UniqueBuffer.hpp>
#include <LangYa/SentryLib/Resource.hpp>

namespace LangYa::SentryLib
{
	/// @brief TripleBuffer is a thread-safe buffer that can be used to transfer data between threads.
	///	Construct this buffer by sizeof(Content) * 3. 
	class TripleBuffer
	{
		/// @brief Represent a resource that records the memory view with flag.
		using ResourceType = Resource<MemoryView>;

		/// @brief Indicate the index of writer, for pushing the element into this buffer.
		std::atomic_char WriterIndex{0};

		/// @brief Indicate the index of reader, for popping the element from this buffer.
		std::atomic_char ReaderIndex{0};

		/// @brief The inner buffer that contains all the data.
		UniqueBuffer InnerBuffer{};

		/// @brief The resources that records the memory view with flag.
		Resource<MemoryView> Resources[3]{};

	public:
		/// @brief Construct a TripleBuffer with the given size.
		///	@param size The count of bytes of this buffer, which means 3 * sizeof(Content). If the size is not 3 * sizeof(Content), the behavior may be undefined.
		explicit TripleBuffer(MemoryView::SizeType size);

		/// @brief Push the given resource into this buffer.
		///	@param destination The destination to pop the buffer.
		///	@return Whether the content is correctly popped.
		bool Pop(const MemoryView& destination);

		/// @brief Pop the resource from this buffer.
		///	@param resource The resource to push into the buffer.
		///	@return Whether the content is correctly pushed.
		void Push(const MemoryView& resource);
	};
}
