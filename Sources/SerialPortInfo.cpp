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
