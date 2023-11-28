#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/SerialPortInfo.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a serial port connection in the system.
	/// Basically a decorator of boost::asio::serial_port.
	class SerialPort final : public Connection
	{
		/// @brief A unique pointer to wrapped boost::asio::serial_port.
		std::unique_ptr<boost::asio::serial_port> UniqueSerialPort;

		/// @brief Indicate the information of this serial port.
		SerialPortInfo Info;

		/// @brief Apply options for the serial port.
		///	@param result Mutable error_code to indicate whether this procedure is normal.
		void ApplyOption(boost::system::error_code& result) const;

		/// @brief Refresh this connection.
		///	@param result Mutable error_code to indicate whether this procedure is normal.
		void RefreshConnection(boost::system::error_code& result);

	public:
		/// @brief Serial port constructor, make sure the serial port info be provided.
		///	@param ioContext The io context for boost asio serial port to operate.
		///	@param info The information of a serial port.
		explicit SerialPort(boost::asio::io_context& ioContext, SerialPortInfo info);

		/// @brief Serial port shared constructor, make sure the serial port info be provided.
		///	@param 
		static std::shared_ptr<SerialPort> BuildShared(boost::asio::io_context& ioContext, const SerialPortInfo& info);

		SerialPort() = delete;

		~SerialPort() override;

		void Connect() override;

		[[nodiscard]] bool IsConnected() override;

		void Disconnect() override;

		[[nodiscard]] const SerialPortInfo& GetInfo() const;

		MemoryView::SizeType Read(const MemoryView& view) override;

		MemoryView::SizeType Write(const MemoryView& view) override;

		std::string ToString() override;
	};
}
