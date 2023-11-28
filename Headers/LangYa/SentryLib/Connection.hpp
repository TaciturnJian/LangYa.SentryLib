#pragma once

#include <string>

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/MemoryIO.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent an IO style connection that can be read/write.
	///	Provide basic abstraction for those with read and write function.
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

