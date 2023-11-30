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
		/// @brief Connect to the target.
		///	Use @code LangYa::SentryLib::IsConnected() @endcode to check the connection status.
		virtual void Connect() = 0;

		/// @brief Get whether the connection is built.
		///	@return True if the connection is built, false otherwise.
		[[nodiscard]] virtual bool IsConnected() = 0;

		/// @brief Disconnect from the target.
		virtual void Disconnect() = 0;

		/// @brief Get the string representation of the connection. Usually some information.
		///	@return Usually some information about this connection.
		[[nodiscard]] virtual std::string ToString();
	};
}

