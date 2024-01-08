#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <ostream>
#include <cstdint>
// ReSharper disable once CppUnusedIncludeDirective
#include <string_view>

#include <boost/asio/buffer.hpp>

#include <LangYa/Common/IFormatByStream.hpp>

namespace LangYa::Common
{
	/// @brief 一个内存视图，存储一块连续内存的头部指针和长度整数，封装内存读写函数。
	/// @details 
	/// # MemoryView 内存视图
	///
	/// ## 类功能
	///
	/// ### 存储内存的头指针和内存长度
	///
	///	### 封装内存读写函数
	///
	struct MemoryView final : IFormatByStream
	{
		/// @brief 代表 Size 的数据类型。
		///	@details 直接采用目前看来最大的整数类型。
		using SizeType = std::uint64_t;

		/// @brief 代表字节类型。
		///	@details 字节类型是视图中的最小单位，偏移量都是对于字节类型来说的。
		using ByteType = unsigned char;

		/// @brief 代表一个内存的开头。
		///	@details 此类型采用了字节类型的指针，便于使用偏移量访问后面的内容。
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
		///	@param head 数组的头，它的元素类型将会自动推断，注意内存长度是 sizeof(*head) * count。
		///	@param count 数组的元素数量，注意内存长度是 sizeof(*head) * count。
		///	@warning 在使用时，经常会想当然认为第二个参数是内存的长度，这是错误的，它应该是数组的元素数量。
		explicit MemoryView(auto* const head, const SizeType& count = 1) :
			// 鬼知道这里应该使用什么类型的强制转化，所以我使用了 C 风格的强制转化。qwq 不会有人读这里的代码吧。
			// ReSharper disable once CppCStyleCast
			Head((ByteType*)head), // NOLINT(clang-diagnostic-cast-qual)
			Size(count * sizeof(*head))
		{
		}

		/// @brief 销毁内存视图，使其无效化。
		///	在销毁时此函数会将内存头部和内存长度都设置为无效值。
		///	（但其实完全没必要对吧）
		~MemoryView() override;

		/// @brief 基于一个内存视图加上偏移构建一个新的内存视图。
		///	@param view 基础的内存视图。
		///	@param offset 内存头部的偏移，只允许正整数。
		///	@param size 新内存视图对应内存的长度，如果为 0 ，那么将设置为基础内存视图的长度减去偏移量。
		///	@warning 如果新内存视图的长度超过基础内存视图的长度，那么新内存视图将可能是无效的（但不会抛出异常，会在某次不正确使用中造成段错误）。
		MemoryView(const MemoryView& view, const SizeType& offset, const SizeType& size = 0);

		/// @brief 提供快速访问某字节的运算符。
		///	注意，内存不一定有效，而且此运算符不会检查索引是否越界。
		///	@param index 字节的下标，从 0 开始。
		///	@return 此字节的引用。
		ByteType& operator[](const SizeType& index) const;

		/// @brief 从目标复制内存直到填满当前视图。
		///	@param head 被读取的内存的头部。
		void ReadFrom(const void* head) const;

		/// @brief 从目标读取内存，直到当前内存被填满或目标内存不足。
		///	@param view 被读取的内存。
		void ReadFrom(const MemoryView& view) const;

		/// @brief 将视图内所有的内存复制到目标地址。
		///	@param head 目标地址的头部。
		void CopyTo(void* head) const;

		/// @brief 将视图内的所有内存复制到目标地址，直到当前内存写完或目标被填满。
		///	@param view 目标内存的视图。
		void CopyTo(const MemoryView& view) const;

		/// @brief 设置整块内存的值。
		///	本质是调用 memset 函数，这个函数在使用过程中很奇怪，Windows 下不用引用头文件，但是 Linux 下必须引用头文件。
		///	索性包装了此内容。
		///	@param byte 内存视图最后会全部为此值。
		void SetValue(ByteType byte) const;

		/// @brief 检查此视图是否有效，即不为空指针或长度大于 0 。
		///	@warning 此检查不能保证对应的内存真正有效，如野指针构造的内容视图仅从此类的角度出发无法判断是否有效。
		///	@return “指针不指向一个空内容” 是否成立。
		[[nodiscard]] bool IsValid() const noexcept;

		explicit operator boost::asio::mutable_buffer() const;

		explicit operator std::string_view() const noexcept;

		/// @brief 使用流格式化此类型。
		///	@details 使用流将视图中的字节转为十六进制整数后存放进 stream 。
		///	@warning 有副作用：会改变流的整数进制为 16 进制。
		///	@param stream 标准库的输出流，用来存放格式化后的内容。
		///	@param option 格式化的设置参数，其中：
		///
		///		- 0：默认格式，输出内容为 "xx xx xx xx" 不包含字符串的双引号。
		///		- 1: 紧凑型 json 数组格式，输出内容为 "[xx,xx,xx,xx]" 不包含字符串的双引号。
		///
		/// 上述格式中的 xx 都代表一个十六进制整数，占用一个或两个空位，且不包含前缀 0x ，当只有一位时，不会使用 0 补齐数字。
		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override;
	};
}
