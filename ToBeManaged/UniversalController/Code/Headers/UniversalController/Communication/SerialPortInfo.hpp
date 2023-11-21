#pragma once

#include <fmt/format.h>

namespace SangoC::Communication::Connections
{
	struct SerialPortInfo
	{
		volatile bool IsConnected{false};
		unsigned char CharacterSize{8};
		unsigned int BaudRate{115200};
		std::string DeviceName{"/dev/ttyUSB0"};

		SerialPortInfo();

		SerialPortInfo(std::string portName, const unsigned int& baudRate, unsigned char characterSize = 8);

		SerialPortInfo(const SerialPortInfo& other);
		SerialPortInfo(SerialPortInfo&& other) noexcept;
		SerialPortInfo& operator=(const SerialPortInfo& other);
		SerialPortInfo& operator=(SerialPortInfo&& other) noexcept;

		std::string ToString() const;
	};
}
