#include <LangYa/SentryLib/Common/SerialPortInfo.hpp>

#include <sstream>

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
		return (std::stringstream{} << *this).str();
	}

	std::ostream& 
	SerialPortInfo::FormatToString(std::ostream& stream) const
	{
		return stream
			<< R"({"IsConnected":)" << IsConnected
			<< R"(,"CharacterSize":)"<< CharacterSize
			<< R"(,"BaudRate":)" << BaudRate
			<< R"(,"DeviceName":")" << DeviceName << "\"}";
	}

	std::ostream& 
	operator<<(std::ostream& stream, const SerialPortInfo& info)
	{
		return info.FormatToString(stream);
	}
}
