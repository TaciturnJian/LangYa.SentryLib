#pragma once

#include <string>

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/MemoryIO.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一个类似 IO 的连接，只提供基本的读写和开启/关闭连接的功能。
	///	一个连接可以是一个文件，也可以是一个网络连接，也可以是一个内存映射文件。
	class Connection : public MemoryIO
	{
	public:
		/// @brief 连接到目标，使用 @code LangYa::SentryLib::IsConnected() @endcode 检查是否成功连接。
		///	@warning 大多数实现都会阻塞当前线程直到连接成功或失败。
		virtual void Connect() = 0;

		/// @brief 获取是否连接到目标
		///	@return 是否连接到目标
		[[nodiscard]] virtual bool IsConnected() = 0;

		/// @brief 从目标断开连接
		///	@warning 大多数实现都会阻塞当前线程直到断开连接成功或失败。
		virtual void Disconnect() = 0;

		/// @brief 获取此连接的字符串格式的信息，通常使用了 fmt 库，性能不算太好。
		///	@return 连接内部的某些信息的字符串格式。
		[[nodiscard]] virtual std::string ToString();
	};
}
