#include <SangoC/Communication/Connections/SerialPortInfo.hpp>

SangoC::Communication::Connections::SerialPortInfo
::SerialPortInfo()
= default;

SangoC::Communication::Connections::SerialPortInfo
::SerialPortInfo(std::string portName, const unsigned& baudRate, const unsigned char characterSize) :
	CharacterSize(characterSize),
	BaudRate(baudRate),
	DeviceName(std::move(portName))
{
}

SangoC::Communication::Connections::SerialPortInfo
::SerialPortInfo(const SerialPortInfo& other)
= default;

SangoC::Communication::Connections::SerialPortInfo
::SerialPortInfo(SerialPortInfo&& other) noexcept
= default;

SangoC::Communication::Connections::SerialPortInfo&
SangoC::Communication::Connections::SerialPortInfo
::operator=(const SerialPortInfo& other)
= default;

SangoC::Communication::Connections::SerialPortInfo& 
SangoC::Communication::Connections::SerialPortInfo
::operator=(SerialPortInfo&& other) noexcept
= default;

std::string
SangoC::Communication::Connections::SerialPortInfo
::ToString() const
{
	return fmt::format(
		R"({{"IsConnected":"{}","CharacterSize":"{}","BaudRate":"{}","DeviceName":"{}"}})",
		IsConnected,
		CharacterSize,
		BaudRate,
		DeviceName
	);
}
