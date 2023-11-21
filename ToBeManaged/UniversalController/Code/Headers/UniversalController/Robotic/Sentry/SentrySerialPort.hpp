#pragma once

#include <memory>
#include <SangoC/Communication/Connections/SerialPort.hpp>
#include <SangoC/Memory/Buffers/PingPongBuffer.hpp>
#include <SangoC/Memory/Buffers/UniqueBuffer.hpp>
#include <SangoC/Robotic/Sentry/CompressedSentrySensorData.hpp>

namespace SangoC::Robotic::Sentry
{
	class SentrySerialPort final : public Communication::Connections::Connection
	{
		std::unique_ptr<Communication::Connections::SerialPort> UniqueSerialPort;
		Memory::Buffers::PingPongBuffer ReaderPingPong{
			Memory::Buffers::UniqueBuffer{sizeof(CompressedSentrySensorData) * 2}
		};

	public:
		void Connect() override
		{
			UniqueSerialPort->Connect();
		}

		[[nodiscard]] bool IsConnected() override
		{
			return UniqueSerialPort->IsConnected();
		}

		void Disconnect() override
		{
			UniqueSerialPort->Disconnect();
		}

		[[nodiscard]] const Communication::Connections::SerialPortInfo& GetInfo() const
		{
			return UniqueSerialPort->GetInfo();
		}

		Memory::MemoryView::SizeType Read(const Memory::MemoryView& view) override
		{
			if (view.Size != sizeof(sizeof(CompressedSentrySensorData))) return 0;

			auto& ping = ReaderPingPong.GetPing();
			UniqueSerialPort->Read(ping);
			if (ReaderPingPong.Exchange(ping, view)) return view.Size;
			return 0;
		}

		Memory::MemoryView::SizeType Write(const Memory::MemoryView& view) override
		{
			return UniqueSerialPort->Write(view);
		}
	};
}
