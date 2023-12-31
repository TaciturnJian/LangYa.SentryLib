#pragma once

#include <LangYa/SentryLib/Common/Connection.hpp>
#include <LangYa/SentryLib/Common/TripleBuffer.hpp>
#include <LangYa/SentryLib/Common/PingPongBuffer.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一个使用狼牙通信方式的连接，用来装饰一个连接。
	///	真正的数据发送过程在下面的函数中 @code LangYa::SentryLib::LangYaConnection::HandleIO @endcode ，此方法阻塞线程，请自行在程序的合适位置调用。
	class LangYaConnection final : public Connection
	{
		/// @brief 接收数据的原始大小（实际大小会多出一个头字节）
		MemoryView::SizeType RawReceiveSize;

		/// @brief 发送数据的原始大小（实际大小会多出一个头字节）
		MemoryView::SizeType RawSendSize;

		/// @brief 内部被装饰的连接。
		std::shared_ptr<Connection> SharedConnection;

		/// @brief 接收数据后存放的三重缓冲区，用于防止接收频率远大于处理频率导致的数据包累积。
		TripleBuffer ReceiveTriple;

		/// @brief 发送数据的三重缓冲区，用于防止发送频率远大于处理频率导致的数据包累积。
		TripleBuffer SendTriple;

		/// @brief 处理内部的发送过程。
		void HandleSending();

		/// @brief 处理内部的接收过程。
		void HandleReceiving();

	public:
		/// @brief 指示当前连接的发送和接收是否正在进行。
		///	使用如下函数开启工作 @code LangYa::SentryLib::LangYaConnection::HandleIO() @endcode.
		///	正在工作时，将其赋值为 false 以停止工作。
		///	至于何时真正停止工作，你需要观察调用 HandleIO 的线程是否不再阻塞在 HandleIO 。
		volatile bool IsWorking{false};

		/// @brief 使用给定的缓冲区大小和内部连接构建一个连接。
		///	@param rawReceiveSize 接收数据的原始大小，最好使用 sizeof()。（此数据需要你和通信对方协商）
		///	@param rawSendSize 发送数据的原始大小，最好使用 sizeof()。（此数据需要你和通信对方协商）
		///	@param weakConnection 内部真正用于通信的连接。
		LangYaConnection(
			MemoryView::SizeType rawReceiveSize, 
			MemoryView::SizeType rawSendSize,
			const std::weak_ptr<Connection>& weakConnection
		);

		/// @brief 处理连接的 IO 内容。
		/// 将会阻塞当前线程，成功启动后 IsWorking 将会被设置为 true ，函数退出时 IsWorking 将会设置为 false。
		void HandleIO();

		/// @brief 从缓冲区中读取一个数据包。数据包的大小一定大于或等于在构造此连接时提供的原始数据大小。
		///	@param view 数据包的大小一定大于或等于在构造此连接时提供的原始数据大小。
		///	@return 如果提供的内存视图的大小较小，则不会读取任何数据，返回 0 ，否则返回读取的数据包的数据大小。
		MemoryView::SizeType Read(const MemoryView& view) override;

		/// @brief 把数据包写入到缓冲区中。
		///	@param view 数据包的内存视图，数据包的大小一定大于或等于在构造此连接时提供的原始数据大小。
		MemoryView::SizeType Write(const MemoryView& view) override;

		/// @brief 让此连接的内部内容开始连接到目标。
		void Connect() override;

		/// @brief Check whether this connection is connected.
		///	@return Whether this connection is connected.
		[[nodiscard]] bool IsConnected() override;

		/// @brief Disconnect and release all the resources.
		void Disconnect() override;
	};
}
