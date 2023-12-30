#pragma once

#include <memory>

#include <LangYa/SentryLib/IOConnection.hpp>
#include <LangYa/SentryLib/IPv4Endpoint.hpp>

namespace LangYa::SentryLib
{
	class IPv4TCPSocket final : public IOConnection
	{
	public:
		using SharedIOContextType = std::shared_ptr<boost::asio::io_context>;
		using SharedTCPSocketType = std::shared_ptr<boost::asio::ip::tcp::socket>;
		using EndpointType = IPv4Endpoint;

	private:
		SharedIOContextType SharedIOContextPtr{nullptr};

		/// @brief 在本机中的端点。
		IPv4Endpoint LocalEndpoint{};

		/// @brief 在远程的端点。
		IPv4Endpoint RemoteEndpoint{};

		SharedTCPSocketType SharedSocketPtr{nullptr};

		void Reconnect();

	public:
		~IPv4TCPSocket() override;

		/// @brief 构建一个空的套接字，它目前不能执行任何有关正常套接字的操作。
		IPv4TCPSocket();

		/// @brief 使用一个可能的本地端点构建一个套接字，构建一个完整的套接字，在本机享有资源，但是不与外界建立连接。
		explicit IPv4TCPSocket(EndpointType targetEndpoint);

		/// @brief 在当前线程创建 IO 上下文，使用指定的本机端点和目标端点构建一个完整的套接字，在本机享有资源，但是不与外界建立连接。
		IPv4TCPSocket(EndpointType localEndpoint, EndpointType targetEndpoint);

		/// @brief 使用给定的 IO 上下文，使用指定的本机端点和目标端点构建一个完整的套接字，构建一个完整的套接字，在本机享有资源，但是不与外界建立连接。
		IPv4TCPSocket(SharedIOContextType sharedIOContext, EndpointType localEndpoint, EndpointType targetEndpoint);

		/// @brief 包装未连接的套接字，指定以后要连接的目标，不与外界建立连接。
		IPv4TCPSocket(SharedTCPSocketType sharedSocket, EndpointType targetEndpoint);

		/// @brief 包装已经建立好连接的套接字。
		explicit IPv4TCPSocket(SharedTCPSocketType sharedSocket);

		MemoryView::SizeType Read(const MemoryView& view) override;

		MemoryView::SizeType Write(const MemoryView& view) override;

		bool Open() override;

		void Close() override;

		std::ostream& GetTargetInfo(std::ostream& stream) override;
	};
}
