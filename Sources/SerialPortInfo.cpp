#include <LangYa/SentryLib/SerialPortInfo.hpp>

LangYa::SentryLib::SerialPortInfo
::SerialPortInfo()
= default;

LangYa::SentryLib::SerialPortInfo
::SerialPortInfo(std::string portName, const unsigned& baudRate, const unsigned char characterSize) :
	CharacterSize(characterSize),
	BaudRate(baudRate),
	DeviceName(std::move(portName))
{
}

LangYa::SentryLib::SerialPortInfo
::SerialPortInfo(const SerialPortInfo& other)
= default;

LangYa::SentryLib::SerialPortInfo
::SerialPortInfo(SerialPortInfo&& other) noexcept
= default;

LangYa::SentryLib::SerialPortInfo&
LangYa::SentryLib::SerialPortInfo
::operator=(const SerialPortInfo& other)
= default;

LangYa::SentryLib::SerialPortInfo& 
LangYa::SentryLib::SerialPortInfo
::operator=(SerialPortInfo&& other) noexcept
= default;

std::string
LangYa::SentryLib::SerialPortInfo
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
