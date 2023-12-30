#pragma once

#include <boost/asio/ip/address.hpp>

#include <LangYa/SentryLib/CanStreamFormatToConsoleFriendlyString.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表 IPv4 地址。
	///	解析字符串的时候小心点，这玩意儿的问题好多好多。
	struct IPv4Address final : CanStreamFormatToConsoleFriendlyString
	{
		/// @brief IPv4 地址中每个位置的数字类型。
		///	@details 因为每个位置的范围是 [0, 255] 的整数，所以采用的是 unsigned char。
		using NumberType = unsigned char;

		/// @brief IPv4 地址中每个位置的数字。
		NumberType Numbers[4]{0};

		/// @brief 默认构造为 0.0.0.0 。
		IPv4Address();

		/// @brief 初始化 IPv4 地址为 a.b.c.d 。
		IPv4Address(NumberType a, NumberType b, NumberType c, NumberType d);

		/// @brief 从字符串中解析 IPv4 地址。
		///	@details 在大多数时候，解析失败都不会抛出异常，请自行规范自己的参数输入。
		///	本质是调用如下函数：
		///	@code LangYa::SentryLib::IPv4Address::Parse(std::istream&) @endcode
		explicit IPv4Address(std::string_view address);

		/// 使用简单的方式获取里面的内容。
		NumberType& operator[](NumberType index);

		/// @brief 从输入流中读取连续四个 unsigned char 作为 IPv4 地址。
		/// @details 找到四个无符号字符整数返回 true ，否则，将会返回 false 。
		///	例如 "192l168?137:1" 和 "192.168.137.1" 都会被解析成 unsigned char[4]{192, 168, 137, 1} 。
		///	@warning 因为作者实际上不会 C++，对 std::istream 完全不了解，所以函数在不正确的参数输入下也可能解析出东西。
		/// @TODO 谁能帮我修一下这个bug QAQ
		bool Parse(std::istream& stream);

		/// @brief 解析字符串填充此地址，内部调用如下函数：
		///	@code LangYa::SentryLib::IPv4Address::Parse(std::istream&) @endcode
		bool Parse(std::string_view address);

		/// @brief 从字符串中解析四个数字。
		///	@details 调用如下函数：
		///	@code LangYa::SentryLib::IPv4Address::Parse(std::istream&) @endcode
		IPv4Address& operator=(std::string_view address);

		/// @brief 从 boost 的地址中获取 IPv4 地址。
		IPv4Address& operator=(const boost::asio::ip::address& address);

		/// @brief 转换为字符串格式的 IPv4 地址。
		///	@details 格式为 [0].[1].[2].[3] 。
		std::ostream& FormatToConsoleFriendlyString(std::ostream& stream) const override;

		/// @brief 转换为 boost 的地址。
		[[nodiscard]] boost::asio::ip::address_v4 ToBoostAddress() const;
	};
}
