#pragma once

#include <boost/asio/ip/tcp.hpp>

#include <LangYa/SentryLib/Network/IPv4Address.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表 IPv4 协议下的目的点。
	/// @details 表达式为 Address:Port 其中 Address 表达为 [0].[1].[2].[3]
	struct IPv4Endpoint final : CanStreamFormatToConsoleFriendlyString
	{
		using PortType = unsigned short;

		IPv4Address Address{};

		PortType Port{0};

		IPv4Endpoint();

		IPv4Endpoint(IPv4Address address, PortType port);

		IPv4Endpoint(std::string_view address, PortType port);

		explicit IPv4Endpoint(std::string_view endpoint);

		bool Parse(std::istream& stream);

		bool Parse(std::string_view endpoint);

		IPv4Endpoint& operator=(std::string_view endpoint);

		IPv4Endpoint& operator=(const boost::asio::ip::tcp::endpoint& endpoint);

		[[nodiscard]] boost::asio::ip::tcp::endpoint ToBoostTCPEndPoint() const;

		std::ostream& FormatToConsoleFriendlyString(std::ostream& stream) const override;
	};
}
