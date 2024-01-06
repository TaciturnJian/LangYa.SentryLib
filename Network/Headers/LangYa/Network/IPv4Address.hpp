#pragma once

#include <cstdint>

#include <boost/asio/ip/address.hpp>

#include <LangYa/Common/IFormatByStream.hpp>
#include <LangYa/Common/IParse.hpp>

namespace LangYa::Network
{
	/// @brief 代表 IPv4 地址，封装了 std::uint8_t[4]。
	///	@details IPv4 的地址顺序是 A.B.C.D，不包含端口，如果需要包含端口请使用 endpoint 。
	///	@warning 解析字符串的时候小心点，这玩意儿的问题好多好多，而且我没想过去修。
	struct IPv4Address final : Common::IFormatByStream, Common::IParseStream, Common::IParseStringView
	{
		/// @brief IPv4 地址中每个位置的数字类型。
		///	@details 因为每个位置的范围是 [0, 255] 的整数，所以采用的是 std::uint8_t。
		using NumberType = std::uint8_t;

		/// @brief IPv4 地址中每个位置的数字。
		NumberType Numbers[4]{0};

#pragma region Constructors

		/// @brief 默认构造为 0.0.0.0 。
		///	@details 内部数组初始化为 0 0 0 0 。
		IPv4Address();

		/// @brief 初始化 IPv4 地址为 a.b.c.d 。
		///	@details 分别存储 a,b,c,d 到内部数组 Numbers 中。
		IPv4Address(NumberType a, NumberType b, NumberType c, NumberType d);

		/// @brief 从字符串中解析 IPv4 地址。
		///	@details 构建一个空的 IPv4 地址，然后再调用 Parse(std::string_view) 。
		///	只图一个方便，可能导致性能损失。
		explicit IPv4Address(std::string_view addressString);

#pragma endregion

#pragma region ParseFromString

		/// @brief 从输入流中读取连续四个 std::uint8_t 作为 IPv4 地址。
		/// @details 找到四个无符号字符整数返回 true ，否则，将会返回 false 。
		///	例如 "192l168?137:1" 和 "192.168.137.1" 都会被解析成 std::uint8_t[4]{192, 168, 137, 1} 。
		///	@warning 因为作者实际上不会 C++，对 std::istream 完全不了解，所以函数在不正确的参数输入下也可能解析出东西。
		/// @TODO 谁能帮我修一下这个bug QAQ
		bool Parse(std::istream& stream, int option = 0) override;

		/// @brief 利用 std::istringstream 转化为 istream 后调用 Parse(std::istream) 。
		bool Parse(std::string_view view, int option = 0) override;

#pragma endregion

#pragma region FormatToString

		/// @brief 转换为字符串格式的 IPv4 地址。
		///	@details 格式为 [0].[1].[2].[3] 。
		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override;

#pragma endregion

#pragma region Converter

		/// @brief 显示转换为 boost::asio::ip::address_v4 。
		///	@details 先转换为字符串，然后调用 boost::asio::ip::make_address_v4 。
		///	有性能损失，如果需要，请自行实现更高效的函数。
		explicit operator boost::asio::ip::address_v4() const;

#pragma endregion

	};
}
