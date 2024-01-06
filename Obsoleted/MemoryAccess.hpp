#pragma once

#include <LangYa/Common/MemoryView.hpp>
#include <LangYa/SentryLib/Common/Connection.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表对与内存的访问，本质是对于 @code LangYa::SentryLib::MemoryView @endcode 的包装。
	class MemoryAccess final : public Connection
	{
		/// @brief 内存视图，指示此连接访问的内存。
		MemoryView View;

	public:
		/// @brief 默认构造函数，提供一个内存视图，此连接将会基于此视图读写内存。
		///	@param view 一个可读写的内存视图。
		explicit MemoryAccess(const MemoryView& view);

		/// @brief 空函数，实现接口。
		void Connect() override;

		/// @brief 指示内存是否有效。
		///	@return 内存是否有效。
		bool IsConnected() override;

		/// @brief 空函数，实现接口。
		void Disconnect() override;

		/// @brief 从内存读取字符，如果内存太小，将读取尽可能多的字符。
		///	@param view 读取字符缓冲区的视图。
		///	@return 读取的字符数量。
		MemoryView::SizeType Read(const MemoryView& view) override;

		/// @brief 向内存填充字符，如果内存不足，将舍弃多余的字符。
		///	@param view 需要填入的字符的视图。
		///	@return 填充的字符数量。
		MemoryView::SizeType Write(const MemoryView& view) override;
	};
}
