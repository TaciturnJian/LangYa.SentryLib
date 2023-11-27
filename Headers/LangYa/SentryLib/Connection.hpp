#pragma once

#include <string>

#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent an IO style connection that can be read/write.
	///	Provide basic abstraction for those with read and write function.
	class Connection
	{
	public:
		/// @brief Destruct a connection, usually make the connection close.
		virtual ~Connection();

		/// @brief Connect to the target.
		///	Use @code LangYa::SentryLib::IsConnected() @endcode to check the connection status.
		virtual void Connect() = 0;

		/// @brief Get whether the connection is built.
		///	@return True if the connection is built, false otherwise.
		[[nodiscard]] virtual bool IsConnected() = 0;

		/// @brief Disconnect from the target.
		virtual void Disconnect() = 0;

		/// @brief Read bytes from the connection.
		///	@param view The view to put the bytes read from the connection.
		///	@return The number of bytes read from the connection.
		virtual MemoryView::SizeType Read(const MemoryView& view) = 0;

		/// @brief Write bytes to the connection.
		///	@param view The view where the bytes to be written are.
		///	@return The number of bytes written to the connection.
		virtual MemoryView::SizeType Write(const MemoryView& view) = 0;

		/// @brief Get the string representation of the connection. Usually some information.
		///	@return Usually some information about this connection.
		[[nodiscard]] virtual std::string ToString();
	};
}

