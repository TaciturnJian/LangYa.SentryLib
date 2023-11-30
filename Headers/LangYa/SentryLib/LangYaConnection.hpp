#pragma once

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/TripleBuffer.hpp>
#include <LangYa/SentryLib/PingPongBuffer.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a connection that send and receive data in lang ya format.
	///	Basically a communication decorator.
	///	@code LangYa::SentryLib::LangYaConnection::HandleIO @endcode calls the really send and receive status and will block the current thread!
	class LangYaConnection final : public Connection
	{
		/// @brief The original size for the content to receive.
		MemoryView::SizeType RawReceiveSize;

		/// @brief The original size for the content to send.
		MemoryView::SizeType RawSendSize;

		/// @brief The inner connection.
		std::shared_ptr<Connection> SharedConnection;

		/// @brief The buffer for receiving data.
		TripleBuffer ReceiveTriple;

		/// @brief The buffer for sending data.
		TripleBuffer SendTriple;

		/// @brief Handle the sending procedure.
		void HandleSending();

		/// @brief Handle the receiving procedure.
		void HandleReceiving();

	public:
		/// @brief Indicate the connection is working or not. Modify to stop the working procedure.
		///	Start working by @code LangYa::SentryLib::LangYaConnection::HandleIO() @endcode.
		volatile bool IsWorking{false};

		/// @brief Construct a lang ya connection by provided arguments.
		///	@param rawReceiveSize The original size for the content to receive.
		///	@param rawSendSize The original size for the content to send.
		///	@param weakConnection The inner connection to use.
		LangYaConnection(
			MemoryView::SizeType rawReceiveSize, 
			MemoryView::SizeType rawSendSize,
			const std::weak_ptr<Connection>& weakConnection
		);

		/// @brief Handle the IO procedure, will block the current thread to start event loop.
		void HandleIO();

		/// @brief Read the package from the buffer.
		///	@param view The view for the buffer, must be equal or bigger than RawReceiveSize.
		///	@return 0 if the size or connection is not ok, otherwise the size of the package.
		MemoryView::SizeType Read(const MemoryView& view) override;

		/// @brief Write the package to the buffer.
		///	@param view The view for the resource, must be equal or bigger than RawSendSize.
		MemoryView::SizeType Write(const MemoryView& view) override;

		/// @brief Make this connection connected.
		void Connect() override;

		/// @brief Check whether this connection is connected.
		///	@return Whether this connection is connected.
		[[nodiscard]] bool IsConnected() override;

		/// @brief Disconnect and release all the resources.
		void Disconnect() override;
	};
}
