#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/SerialPortInfo.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表系统中的一个串口，本质是 @code boost::asio::serial_port @endcode 的装饰器。
	///	提供了 @code LangYa::SentryLib::Connection @endcode 的实现。
	class SerialPort final : public Connection
	{
		/// @brief 指向包装的串口的独有指针。
		std::unique_ptr<boost::asio::serial_port> UniqueSerialPort;

		/// @brief 指示串口的信息。
		SerialPortInfo Info;

		/// @brief 为串口应用设置。
		///	@param result 传入可修改的错误码，用于指示是否成功应用设置。
		void ApplyOption(boost::system::error_code& result) const;

		/// @brief 刷新当前连接。
		///	@param result 传入可修改的错误码，用于指示是否成功应用设置。
		void RefreshConnection(boost::system::error_code& result);

	public:
		/// @brief 构建串口，请提供有效的串口信息。
		///	@param ioContext boost::asio::io_context 对象。
		///	@param info 串口的信息。
		explicit SerialPort(boost::asio::io_context& ioContext, SerialPortInfo info);

		/// @brief 构建串口，返回共享串口指针，请提供有效的串口信息。
		///	@param ioContext boost::asio::io_context 对象。
		///	@param info 串口的信息。
		///	@return 共享串口指针。
		static std::shared_ptr<SerialPort> BuildShared(boost::asio::io_context& ioContext, const SerialPortInfo& info);

		/// @brief 禁止默认构造
		SerialPort() = delete;

		/// @brief 禁止拷贝构造。
		SerialPort(const SerialPort&) = delete;

		/// @brief 禁止拷贝赋值。
		SerialPort& operator=(const SerialPort&) = delete;

		/// @brief 禁止移动构造。
		SerialPort(SerialPort&&) = delete;

		/// @brief 禁止移动赋值。
		SerialPort& operator=(SerialPort&&) = delete;

		/// @brief 清理串口占用的资源。
		~SerialPort() override;

		/// @brief 连接到串口，或者称为打开串口。
		void Connect() override;

		/// @brief 检查当前串口是否已经连接。
		///	@return 当前串口是否已经连接。
		[[nodiscard]] bool IsConnected() override;

		/// @brief 断开当前串口的连接。
		void Disconnect() override;

		/// @brief 获取串口信息。
		///	@return 不可修改的串口信息。
		[[nodiscard]] const SerialPortInfo& GetInfo() const;

		MemoryView::SizeType Read(const MemoryView& view) override;

		MemoryView::SizeType Write(const MemoryView& view) override;

		std::string ToString() override;
	};
}
