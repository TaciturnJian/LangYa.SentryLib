#pragma once

#include <LangYa/Common/MemoryView.hpp>

namespace LangYa::Common
{
	/// @brief 代表一个基本的读写类，提供基本的 IO 功能。
	/// @details 在读和写的时候使用如下类作为函数参数 @code LangYa::Common::MemoryView @endcode 。
	///	一般来说读取操作为 Read/Receive ，写入操作为 Write/Send 。
	class MemoryIO
	{
	public:
		using SizeType = MemoryView::SizeType;

		/// @brief 默认虚析构函数。
		virtual ~MemoryIO();

		/// @brief 读取字节（最多读取 view 的所有字节）。
		///	@param view 缓冲区的内存视图，读取到的字节将存放在这个视图对应的内存中。
		///	@warning 此函数的实现一般不会检查视图是否有效，所以调用者必须自己保证视图有效。
		///	@return 读取并存放了多少个字节。
		virtual SizeType Read(const MemoryView& view) = 0;

		/// @brief 写入字节（最多写入 view 的所有字节）
		///	@param view 资源的内存视图，将要写入的内存应当放在这个视图对应的内存中。
		///	@warning 此函数的实现一般不会检查视图是否有效，所以调用者必须自己保证视图有效。
		///	@return 写入了多少个字节。
		virtual SizeType Write(const MemoryView& view) = 0;
	};
}
