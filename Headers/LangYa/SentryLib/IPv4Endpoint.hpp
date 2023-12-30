#pragma once

#include <boost/asio/ip/tcp.hpp>

#include <LangYa/SentryLib/CanStreamFormatToJson.hpp>
#include <LangYa/SentryLib/IPv4Address.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表 IPv4 协议下的目的点。
	/// @details 表达式为 Address:Port 其中 Address 表达为 [0].[1].[2].[3]
	struct IPv4Endpoint final : CanStreamFormatToJson
	{
		using PortType = unsigned short;

		IPv4Address Address{};

		PortType Port{0};

		[[nodiscard]] boost::asio::ip::tcp::endpoint ToBoostTCPEndPoint() const
		{
			return {boost::asio::ip::make_address(FormatToString(Address)), Port};
		}

		std::ostream& FormatToJson(std::ostream& stream) const override
		{
			return stream << R"({"Address":)" << Address << R"(,"Port":)" << Port << '}';
		}
	};
}
