#include <sstream>

#include <LangYa/Network/SerialPortInfo.hpp>

namespace LangYa::Network
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

	std::ostream& 
	SerialPortInfo::FormatByStream(std::ostream& stream, const int option) const
	{
		return stream
			<< R"({"IsConnected":)" << IsConnected
			<< R"(,"CharacterSize":)"<< CharacterSize
			<< R"(,"BaudRate":)" << BaudRate
			<< R"(,"DeviceName":")" << DeviceName << "\"}";
	}
}
