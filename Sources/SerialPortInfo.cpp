#include <LangYa/SentryLib/SerialPortInfo.hpp>

namespace LangYa::SentryLib
{
	SerialPortInfo
	::SerialPortInfo()
	= default;

	SerialPortInfo
	::SerialPortInfo(std::string portName, const unsigned& baudRate, const unsigned char characterSize) :
		CharacterSize(characterSize),
		BaudRate(baudRate),
		DeviceName(std::move(portName))
	{
	}

	SerialPortInfo
	::SerialPortInfo(const SerialPortInfo& other)
	= default;

	SerialPortInfo
	::SerialPortInfo(SerialPortInfo&& other) noexcept
	= default;

	SerialPortInfo&
	SerialPortInfo
	::operator=(const SerialPortInfo& other)
	= default;

	SerialPortInfo&
	SerialPortInfo
	::operator=(SerialPortInfo&& other) noexcept
	= default;

	std::string
	SerialPortInfo
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
}
