#pragma once

#include <boost/asio/ip/tcp.hpp>

#include <LangYa/SentryLib/Network/IPv4Address.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表 IPv4 协议下的目的点。
	/// @details 表达式为 Address:Port 其中 Address 表达为 [0].[1].[2].[3]
	struct IPv4Endpoint final : IFormatByStream, IParseStream, IParseStringView
	{
		using PortType = unsigned short;

		IPv4Address Address{};

		PortType Port{0};

		IPv4Endpoint();

		IPv4Endpoint(IPv4Address address, PortType port);

		IPv4Endpoint(std::string_view address, PortType port);

		explicit IPv4Endpoint(std::string_view endpoint);

		explicit IPv4Endpoint(const boost::asio::ip::tcp::endpoint& endpoint);

		bool Parse(std::istream& stream, int option = 0) override;

		bool Parse(std::string_view view, int option = 0) override;

		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override;

		explicit operator boost::asio::ip::tcp::endpoint() const;
	};
}
