#pragma once

#include <fmt/format.h>

namespace LangYa::SentryLib
{
	/// @brief The essential component for building a serial port.
	/// Contains the information of a serial port connection.
	struct SerialPortInfo
	{
		/// @brief Indicate the connection status of a serial port
		volatile bool IsConnected{false};

		/// @brief Indicate the character size of a serial port connection, by default 8 (== sizeof(char)).
		unsigned char CharacterSize{8};

		/// @brief Indicate the baud rate of a serial port connection, by default 115200.
		unsigned int BaudRate{115200};

		/// @brief Indicate the device of the serial port to use, by default COM1.
		std::string DeviceName{"COM1"};

		/// @brief Default constructor.
		SerialPortInfo();

		/// @brief Construct a serial port with the given information.
		///	@param portName The device name of the serial port to use.
		///	@param baudRate The baud rate of a serial port.
		///	@param characterSize The character size when transferring a byte.
		SerialPortInfo(std::string portName, const unsigned int& baudRate, unsigned char characterSize = 8);

		/// @brief Default copy constructor
		///	@param other The information to copy from.
		SerialPortInfo(const SerialPortInfo& other);

		/// @brief Default move constructor
		///	@param other The information to move.
		SerialPortInfo(SerialPortInfo&& other) noexcept;

		/// @brief Default copy assignment operator.
		///	@param other The information to copy from.
		SerialPortInfo& operator=(const SerialPortInfo& other);

		/// @brief Default move assignment operator.
		///	@param other The information to move.
		SerialPortInfo& operator=(SerialPortInfo&& other) noexcept;

		/// @brief Used to log this.
		[[nodiscard]] std::string ToString() const;
	};
}
