
#include <spdlog/spdlog.h>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <LangYa/Network/IPv4TCPSocket.hpp>

namespace LangYa::Network
{
	void
	IPv4TCPSocket
	::Reconnect()
	{
		if (!Open())
		{
			spdlog::warn(
				"IPv4TCPSocket> Socket({}->{}) failed to reconnect.",
				FormatToString(LocalEndpoint),
				FormatToString(RemoteEndpoint)
			);
		}
	}

	IPv4TCPSocket
	::~IPv4TCPSocket()
	{
		Close();
	}

	IPv4TCPSocket
	::IPv4TCPSocket() = default;

	IPv4TCPSocket
	::IPv4TCPSocket(EndpointType targetEndpoint):
		SharedIOContextPtr(std::make_shared<boost::asio::io_context>()),
		RemoteEndpoint(std::move(targetEndpoint)),
		SharedSocketPtr(
			std::make_shared<boost::asio::ip::tcp::socket>(
				*SharedIOContextPtr, boost::asio::ip::tcp::v4()
			)
		)
	{
		boost::system::error_code result{};
		LocalEndpoint = IPv4Endpoint(SharedSocketPtr->local_endpoint(result));
		if (result.failed())
		{
			spdlog::warn(
				"IPv4TCPSocket> Failed to get local endpoint: {}",
				result.to_string()
			);
		}
	}

	IPv4TCPSocket
	::IPv4TCPSocket(EndpointType localEndpoint, EndpointType targetEndpoint):
		SharedIOContextPtr(std::make_shared<boost::asio::io_context>()),
		LocalEndpoint(std::move(localEndpoint)),
		RemoteEndpoint(std::move(targetEndpoint)),
		SharedSocketPtr(
			std::make_shared<boost::asio::ip::tcp::socket>(
				*SharedIOContextPtr, 
				static_cast<boost::asio::ip::tcp::endpoint>(LocalEndpoint)
			)
		)
	{
	}

	IPv4TCPSocket
	::IPv4TCPSocket(SharedIOContextType sharedIOContext, EndpointType localEndpoint, EndpointType targetEndpoint):
		SharedIOContextPtr(std::move(sharedIOContext)),
		LocalEndpoint(std::move(localEndpoint)),
		RemoteEndpoint(std::move(targetEndpoint)),
		SharedSocketPtr(
			std::make_shared<boost::asio::ip::tcp::socket>(
				*SharedIOContextPtr,
				static_cast<boost::asio::ip::tcp::endpoint>(LocalEndpoint)
			)
		)
	{
	}

	IPv4TCPSocket
	::IPv4TCPSocket(SharedTCPSocketType sharedSocket, EndpointType targetEndpoint):
		RemoteEndpoint(std::move(targetEndpoint)),
		SharedSocketPtr(std::move(sharedSocket))
	{
		LocalEndpoint = IPv4Endpoint(SharedSocketPtr->local_endpoint());
	}

	IPv4TCPSocket
	::IPv4TCPSocket(SharedTCPSocketType sharedSocket):
		SharedSocketPtr(std::move(sharedSocket))
	{
		LocalEndpoint = IPv4Endpoint(SharedSocketPtr->local_endpoint());
		RemoteEndpoint = IPv4Endpoint(SharedSocketPtr->remote_endpoint());
	}

	Common::MemoryView::SizeType
	IPv4TCPSocket
	::Read(const Common::MemoryView& view)
	{
		boost::system::error_code result{};
		auto bytes = read(*SharedSocketPtr, static_cast<boost::asio::mutable_buffer>(view), result);

		// 查看是否读取失败
		if (result.failed() || bytes != view.Size)
		{
			// 打印失败信息
			spdlog::warn(
				"IPv4TCPSocket> Socket({}<-{}) read ({}/{})bytes: {}",
				FormatToString(LocalEndpoint),
				FormatToString(RemoteEndpoint),
				bytes,
				view.Size,
				result.to_string()
			);

			Reconnect();
		}

		return bytes;
	}

	Common::MemoryView::SizeType
	IPv4TCPSocket
	::Write(const Common::MemoryView& view)
	{
		boost::system::error_code result{};
		auto bytes = write(*SharedSocketPtr, static_cast<boost::asio::mutable_buffer>(view), result);

		// 查看是否发送失败
		if (result.failed() || bytes != view.Size)
		{
			// 打印失败信息
			spdlog::warn(
				"IPv4TCPSocket> Socket({}->{}) sent ({}/{})bytes: {}",
				FormatToString(LocalEndpoint),
				FormatToString(RemoteEndpoint),
				bytes,
				view.Size,
				result.to_string()
			);

			Reconnect();
		}
		return bytes;
	}

	bool
	IPv4TCPSocket
	::Open()
	{
		const auto target = static_cast<boost::asio::ip::tcp::endpoint>(RemoteEndpoint);
		const auto target_info = FormatToString(RemoteEndpoint);

		constexpr auto max_attempt = 5;

		for (auto i = 0; i <= max_attempt; i++)
		{
			if (i == max_attempt)
			{
				spdlog::warn(
					"IPv4TCPSocket> Give up connection to({}) after {} attempts.",
					target_info,
					max_attempt
				);
				return false;
			}

			boost::system::error_code result{};
			(void)SharedSocketPtr->connect(target, result);

			if (result.failed())
			{
				spdlog::warn(
					"IPv4TCPSocket> Failed to connect to the target({}), attempt ({}/{}): {}",
					target_info,
					i,
					max_attempt,
					result.to_string()
				);
				continue;
			}
			break;
		}

		spdlog::info(
			"IPv4TCPSocket> Built connection({}->{})",
			FormatToString(LocalEndpoint),
			FormatToString(RemoteEndpoint)
		);

		return true;
	}

	void
	IPv4TCPSocket
	::Close()
	{
		boost::system::error_code result{};
		(void)SharedSocketPtr->close(result);

		if (result.failed())
		{
			spdlog::warn(
				"IPv4TCPSocket> Failed to close the socket({}->{}): {}",
				FormatToString(LocalEndpoint),
				FormatToString(RemoteEndpoint),
				result.to_string()
			);
		}
	}

	std::ostream&
	IPv4TCPSocket
	::GetTargetInfo(std::ostream& stream)
	{
		return stream << LocalEndpoint << "->" << RemoteEndpoint;
	}
}
