#pragma once

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/Common/UniqueBuffer.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一个二重字节缓冲区，用于交换数据并保证最终得到的数据一定以 '!' 开头。
	///	@warning 不是线程安全的类型。
	class PingPongBuffer
	{
		/// @brief 缓冲区的前半段。
		MemoryView Ping{};

		/// @brief 缓冲区的后半段。
		MemoryView Pong{};

		/// @brief 缓冲区的总内存。
		UniqueBuffer InnerBuffer{};

 	public:
		/// @brief Construct a new PingPongBuffer object with provided buffer size.
		///	@param size The size the buffer use. You should use sizeof(element) * 2.
		explicit PingPongBuffer(MemoryView::SizeType size);

		/// @brief Construct a new PingPongBuffer object with provided unique buffer.
		/// Its size must be sizeof(element) * 2.
		///	@param memory The unique buffer moved in.
		explicit PingPongBuffer(UniqueBuffer&& memory);

		/// @brief Deleted copy constructor.
		///	Do not copy ping pong buffer for the consideration of unique memory usage.
		PingPongBuffer(const PingPongBuffer&) = delete;

		/// @brief Default move constructor.
		/// @param other Buffer to be moved.
		PingPongBuffer(PingPongBuffer&& other) noexcept;

		/// @brief Deleted copy assign operator.
		/// Do not copy ping pong buffer for the consideration of unique memory usage.
		PingPongBuffer& operator=(const PingPongBuffer&) = delete;

		/// @brief Default move constructor.
		///	@param other Buffer to be moved.
		PingPongBuffer& operator=(PingPongBuffer&& other) noexcept;

		/// @brief Get ping view of this buffer. Used for debug.
		///	@returns A view of ping buffer.
		[[nodiscard]] const MemoryView& GetPing() const;

		/// @brief Get pong view of this buffer. Used for debug.
		///	@return A view of pong buffer.
		[[nodiscard]] const MemoryView& GetPong() const;

		/// @brief Input the resource, try to read bytes and then write them to destination.
		///	To make sure the destination start with a head flag '!'.
		/// You can follow this example:
		/// @code
		///	SomeUnstableBuffer unstable_buffer;
		///	TargetStableBuffer stable_buffer;
		///	if (!Exchange(unstable_buffer, stable_buffer))
		///	{
		///		//Do some work about this exception.
		///	}
		///	@endcode
		///	@warning Not thread-safe. You should make sure the method is called in a single thread.
		/// @param resource The resource to be read. The head of the resource must be started with a '!'.
		///	@param destination Where the result should be stored.
		///	@return Whether this buffer get a resource from the ping pong buffer.
		[[nodiscard]] bool Exchange(const MemoryView& resource, const MemoryView& destination) const;

		/// @brief Get the exchange buffer for exchange, basically the pong buffer.
		///	@return A view of exchange buffer.
		[[nodiscard]] const MemoryView& GetExchangeBuffer() const noexcept;

		/// @brief Assume the resource has been inputted after
		/// @code LangYa::SentryLib::PingPongBuffer::GetExchangeBuffer() @endcode.
		/// Try to read bytes and then write them to destination.
		///	To make sure the destination start with a head flag '!'.
		///	@warning Not thread-safe. You should make sure the method is called in a single thread.
		///	@param destination Where the result should be stored.
		///	@return Whether this buffer get a resource from the ping pong buffer.
		[[nodiscard]] bool ExchangeWithExchangeBuffer(const MemoryView& destination) const;
	};
}
