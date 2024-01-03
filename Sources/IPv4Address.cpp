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
	::IPv4Address(const std::string_view addressString)
	{
		if (!Parse(addressString))
		{
			throw std::invalid_argument("IPv4Address> Cannot parse address string!"); // TODO 指定异常消息
		}
	}

	bool
	IPv4Address
	::Parse(std::istream& stream, int option)
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
	::Parse(const std::string_view view, const int option)
	{
		std::istringstream stream{std::string{view}};
		return Parse(stream, option);
	}

	std::ostream&
	IPv4Address
	::FormatByStream(std::ostream& stream, int option) const
	{
		return stream
			<< static_cast<int>(Numbers[0]) << '.'
			<< static_cast<int>(Numbers[1]) << '.'
			<< static_cast<int>(Numbers[2]) << '.'
			<< static_cast<int>(Numbers[3]);
	}

	IPv4Address
	::operator
	boost::asio::ip::address_v4() const
	{
		return boost::asio::ip::make_address_v4(
			FormatToString(
				*this
			)
		);
	}
}
