#pragma once

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include <SangoC/Communication/Connections/Connection.hpp>
#include <SangoC/Communication/Connections/SerialPortInfo.hpp>

namespace SangoC::Communication::Connections
{
	class SerialPort final : public Connection
	{
		std::unique_ptr<boost::asio::serial_port> UniqueSerialPort;

		SerialPortInfo Info;

		explicit SerialPort(boost::asio::io_context& ioContext, SerialPortInfo info);

		void ApplyOption(boost::system::error_code& result) const;

		void RefreshConnection(boost::system::error_code& result);

	public:
		static std::shared_ptr<SerialPort> BuildShared(boost::asio::io_context& ioContext, const SerialPortInfo& info);

		SerialPort() = delete;

		~SerialPort() override;

		void Connect() override;

		[[nodiscard]] bool IsConnected() override;

		void Disconnect() override;

		[[nodiscard]] const SerialPortInfo& GetInfo() const;

		Memory::MemoryView::SizeType Read(const Memory::MemoryView& view) override;

		Memory::MemoryView::SizeType Write(const Memory::MemoryView& view) override;

		std::string ToString() override;
	};
}
