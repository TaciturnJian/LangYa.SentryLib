#include <sstream>
#include <istream>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Network/IPv4Address.hpp>

namespace LangYa::SentryLib
{
	IPv4Address
	::IPv4Address() = default;

	IPv4Address
	::IPv4Address(const NumberType a, const NumberType b, const NumberType c, const NumberType d):
		Numbers{a, b, c, d}
	{
	}

	IPv4Address
	::IPv4Address(const std::string_view address)
	{
		*this = address;
	}

	IPv4Address::NumberType&
	IPv4Address
	::operator[](const NumberType index)
	{
		return Numbers[index];
	}

	IPv4Address&
	IPv4Address
	::operator=(const std::string_view address)
	{
		if (!Parse(address))
		{
			throw std::exception(); // TODO 指定异常消息
		}

		return *this;
	}

	IPv4Address&
	IPv4Address
	::operator=(const boost::asio::ip::address& address)
	{
		if (!address.is_v4())
		{
			spdlog::warn("IPv4Address> Cannot get address for ipv4 from an address not ipv4");
			return *this;
		}

		*this = address.to_v4().to_string();
		return *this;
	}

	bool
	IPv4Address
	::Parse(std::istream& stream)
	{
		try
		{
			int temp{0};
			char separator{};
			for (auto& num : Numbers)
			{
				stream >> temp >> separator;
				num = static_cast<NumberType>(temp);
			}
			return true;
		}
		catch (const std::exception& ex)
		{
			spdlog::warn("IPv4Address> Failed when parsing address from stream: {}", ex.what());
			return false;
		}
		catch (...)
		{
			spdlog::warn("IPv4Address> Failed when parsing address from stream: Unknown exception");
			return false;
		}
	}

	bool
	IPv4Address
	::Parse(std::string_view address)
	{
		std::stringstream stream{address.data()};
		return Parse(stream);
	}

	std::ostream&
	IPv4Address
	::FormatToConsoleFriendlyString(std::ostream& stream) const
	{
		return stream
			<< static_cast<int>(Numbers[0]) << '.'
			<< static_cast<int>(Numbers[1]) << '.'
			<< static_cast<int>(Numbers[2]) << '.'
			<< static_cast<int>(Numbers[3]);
	}

	boost::asio::ip::address_v4
	IPv4Address
	::ToBoostAddress() const
	{
		return boost::asio::ip::make_address_v4(SentryLib::FormatToConsoleFriendlyString(*this));
	}
}
