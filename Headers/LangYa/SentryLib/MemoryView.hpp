#pragma once

#include <ostream>

#include <boost/asio/buffer.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一个内存的视图，保存内存的头指针和长度，供其他读写。
	/// 不能保证作为传入的参数时得到的内存视图是有效的，因此作为函数参数时，这个类的基础是对调用者的信任。
	struct MemoryView
	{
		/// @brief 代表 Size 的数据类型。
		using SizeType = unsigned long long;

		/// @brief 代表字节类型。
		using ByteType = unsigned char;

		/// @brief 代表一个内存的开头。
		using HeadType = ByteType*;

		/// @brief 记录内存的开头，可以使用偏移量访问后面的内容。
		/// @warning 当它为 nullptr 时，此内存视图无效。
		HeadType Head{nullptr};

		/// @brief 记录内存的长度，使用偏移量时访问内存时不应该超过此长度。
		///	@warning 当它为 0 时，此内存视图无效。
		SizeType Size{0};

		/// @brief 构建一个完全无效的内存视图。
		///	其内存的头部为 nullptr ，内存长度为 0。
		MemoryView() noexcept;

		/// @brief 使用给定的内存头和内存长度构建一个内存视图。
		///	如果提供的两个参数是非法的，那么此内存视图也是非法的，而且此类不会在构建时抛出异常。小心在之后的使用中出现段错误。
		///	@tparam THead 头部，将会从参数 head 自动推断。
		///	@param head 数组的头，它的元素类型将会自动推断，注意内存长度是 sizeof(*head) * count。
		///	@param count 数组的元素数量，注意内存长度是 sizeof(*head) * count。
		///	@warning 在使用时，经常会想当然认为第二个参数是内存的长度，这是错误的，它应该是数组的元素数量。
		template <typename THead>
		explicit MemoryView(THead* const head, const SizeType& count = 1) :
			// 鬼知道这里应该使用什么类型的强制转化，所以我使用了 C 风格的强制转化。qwq 不会有人读这里的代码吧。
			// ReSharper disable once CppCStyleCast
			Head((ByteType*)head),
			Size(count * sizeof(THead))
		{
		}

		/// @brief 基于一个内存视图加上偏移构建一个新的内存视图。
		///	@param view 基础的内存视图。
		///	@param offset 内存头部的偏移，只允许正整数。
		///	@param size 新内存视图对应内存的长度，如果为 0 ，那么将设置为基础内存视图的长度减去偏移量。
		///	@warning 如果新内存视图的长度超过基础内存视图的长度，那么新内存视图将可能是无效的（但不会抛出异常，会在某次不正确使用中造成段错误）。
		MemoryView(const MemoryView& view, const SizeType& offset, const SizeType& size = 0);

		/// @brief 默认的拷贝构造函数，复制头指针和长度。
		///	@param other 被拷贝的内存，不论是否有效。
		MemoryView(const MemoryView& other);

		/// @brief Default move constructor.
		///	@param other The moved one.
		MemoryView(MemoryView&& other) noexcept;

		/// @brief Default copy constructor.
		///	@param other The copied one.
		///	@return The reference of this.
		MemoryView& operator=(const MemoryView& other);

		/// @brief Default move constructor.
		///	@param other The moved one.
		///	@return The reference of this.
		MemoryView& operator=(MemoryView&& other) noexcept;

		/// @brief Access memory like byte array.
		///	@param index The index of the byte.
		///	@return The reference of the byte.
		ByteType& operator[](const SizeType& index) const;

		/// @brief Read the given memory started with the head, fill all memory for this memory view, basically a wrap for memcpy.
		///	@param head The head of the memory.
		///	@return The size of transferred bytes.
		void ReadFrom(const void* head) const;

		/// @brief Read the given view. The target byte count is the min value of this->Size and view.Size.
		///	@param view The view to read.
		///	@return The size of transferred bytes.
		void ReadFrom(const MemoryView& view) const;

		/// @brief Write bytes to the given memory started with the head, fill all memory for this memory view, basically a wrap for memcpy.
		///	@param head The head of the memory.
		///	@return The size of transferred bytes.
		void CopyTo(void* head) const;

		/// @brief Write bytes to the given view. The target byte count is the min value of this->Size and view.Size.
		///	@param view The view to write to.
		///	@return The size of transferred bytes.
		void CopyTo(const MemoryView& view) const;

		/// @brief Destruct the memory view.
		///	Set the head to nullptr and set the size to zero.
		~MemoryView();

		/// @brief Check if the memory view is valid, basically check the head and the size.
		///	@warning The result true cannot guarantee the memory is valid.
		///	@return true if the memory view is valid, otherwise false.
		[[nodiscard]] bool IsValid() const noexcept;

		/// @brief Print the byte in int into the stream.
		///	@param stream Where to put the bytes.
		///	@param view Where the bytes are.
		///	@return The reference same of the parameter stream.
		friend std::ostream& operator<<(std::ostream& stream, const MemoryView& view)
		{
			for (SizeType i = 0; i < view.Size; i++)
			{
				stream << static_cast<int>(view[i]) << ' ';
			}

			return stream;
		}

		/// @brief Convert this view to boost::asio::mutable_buffer.
		///	@return A instance of boost::asio::mutable_buffer.
		[[nodiscard]] boost::asio::mutable_buffer ToBuffer() const;
	};
}
