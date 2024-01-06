#pragma once

#include <string>

#include <LangYa/Common/IFormatByStream.hpp>

namespace LangYa::Network
{
	/// @brief 构建串口连接的必要内容。
	/// 包含了构建串口的必要信息，许多复杂设置都略去了。
	///	请配合下面的类使用。
	///	@code LangYa::Network::SerialPort @endcode
	struct SerialPortInfo final : Common::IFormatByStream
	{
		/// @brief 指示串口的状态。
		volatile bool IsConnected{false};

		/// @brief 串口的字节大小。
		/// 默认为 C语言标准 (一字节 8 个二进制位) 。
		unsigned char CharacterSize{8};

		/// @brief 串口的波特率。
		/// 默认是 115200 。
		unsigned int BaudRate{115200};

		/// @brief 指示串口使用的设备名称。
		///	默认为 Windows 下的设备 "COM1"。
		std::string DeviceName{"COM1"};

		/// @brief 默认构造函数。
		SerialPortInfo();

		/// @brief 使用指定的参数构造一个串口信息。
		///	@param portName 串口使用的设备名称。
		///	@param baudRate 串口的波特率。
		///	@param characterSize 串口的字节大小。
		SerialPortInfo(std::string portName, const unsigned int& baudRate, unsigned char characterSize = 8);

		/// @brief 将串口信息转换为 json 格式字符串。
		///	用于日志。
		[[nodiscard]] std::string ToString() const;

		/// @brief 将串口信息转换为 json 格式字符串。
		///	用于日志。
		///	@param stream 流输出。
		///	@return 参数中流输出的引用。
		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override;

		/// @brief 将串口信息转换为 json 格式字符串。
		///	用于日志。
		///	@param stream 流输出。
		///	@param info 串口信息。
		///	@return 参数中流输出的引用。
		friend std::ostream& operator<<(std::ostream& stream, const SerialPortInfo& info);
	};
}
