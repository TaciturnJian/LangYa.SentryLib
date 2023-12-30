#include <LangYa/SentryLib/IPv4Endpoint.hpp>

namespace LangYa::SentryLib
{
	IPv4Endpoint
	::IPv4Endpoint() = default;

	IPv4Endpoint
	::IPv4Endpoint(IPv4Address address, const PortType port) :
		Address(std::move(address)),
		Port(port)
	{
	}

	IPv4Endpoint::IPv4Endpoint(const std::string_view address, const PortType port):
		Address(address),
		Port(port)
	{
	}

	IPv4Endpoint
	::IPv4Endpoint(const std::string_view endpoint)
	{
		*this = endpoint;
	}

	IPv4Endpoint&
	IPv4Endpoint
	::operator=(const std::string_view endpoint)
	{
		if (!Parse(endpoint))
		{
			throw std::exception(); // TODO 指定异常消息
		}

		return *this;
	}

	IPv4Endpoint&
	IPv4Endpoint
	::operator=(const boost::asio::ip::tcp::endpoint& endpoint)
	{
		Address = endpoint.address();
		Port = endpoint.port();
		return *this;
	}

	bool
	IPv4Endpoint
	::Parse(std::istream& stream)
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
	::Parse(std::string_view endpoint)
	{
		std::stringstream stream{endpoint.data()};
		return Parse(stream);
	}

	boost::asio::ip::tcp::endpoint
	IPv4Endpoint
	::ToBoostTCPEndPoint() const
	{
		return {Address.ToBoostAddress(), Port};
	}

	std::ostream&
	IPv4Endpoint
	::FormatToConsoleFriendlyString(std::ostream& stream) const
	{
		return Address.FormatToConsoleFriendlyString(stream) << ':' << Port;
	}
}
