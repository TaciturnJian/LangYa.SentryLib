#include <spdlog/spdlog.h>

#include <LangYa/Network/IPv4Endpoint.hpp>

namespace LangYa::Network
{
	IPv4Endpoint
	::IPv4Endpoint() = default;

	IPv4Endpoint
	::IPv4Endpoint(IPv4Address address, const PortType port) :
		Address(std::move(address)),
		Port(port)
	{
	}

	IPv4Endpoint
	::IPv4Endpoint(const std::string_view address, const PortType port):
		Address(address),
		Port(port)
	{
	}

	IPv4Endpoint
	::IPv4Endpoint(const std::string_view endpoint)
	{
		if (!Parse(endpoint))
		{
			throw std::exception(); // TODO 指定异常消息
		}
	}

	IPv4Endpoint
	::IPv4Endpoint(const boost::asio::ip::tcp::endpoint& endpoint)
	{
		if (!Address.Parse(endpoint.address().to_string()))
		{
			throw std::exception(); // TODO 指定异常消息
		}
		Port = endpoint.port();
	}

	bool
	IPv4Endpoint
	::Parse(std::istream& stream, int option)
	{
		if (!Address.Parse(stream)) return false;

		try
		{
			stream >> Port;
			return true;
		}
		catch (const std::exception& ex)
		{
			spdlog::warn("IPv4Endpoint> Failed when parsing port from stream: {}", ex.what());
			return false;
		}
		catch (...)
		{
			spdlog::warn("IPv4Endpoint> Failed when parsing port from stream: Unknown exception");
			return false;
		}
	}

	bool
	IPv4Endpoint
	::Parse(const std::string_view view, int option)
	{
		std::istringstream stream{std::string{view}};
		return Parse(stream);
	}

	std::ostream&
	IPv4Endpoint
	::FormatByStream(std::ostream& stream, int option) const
	{
		return Address.FormatByStream(stream) << ':' << Port;
	}

	IPv4Endpoint
	::operator
	boost::asio::ip::tcp::endpoint() const
	{
		return {static_cast<boost::asio::ip::address_v4>(Address), Port};
	}
}
