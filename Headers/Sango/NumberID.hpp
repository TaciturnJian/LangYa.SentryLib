#pragma once

#include <cstdint>

#include <Sango/IComparable.hpp>

namespace Sango
{
	/// @warning 使用小数作为 ID 的人，自裁吧。用小数可能比较大小是对的，但是等号比较必错。 :)
	template <typename TNumber>
	struct NumberID final : IComparable<NumberID<TNumber>>
	{
		TNumber Value{};

		NumberID() = default;

		explicit NumberID(TNumber number) : Value(number)
		{
		}

		[[nodiscard]] CompareResult CompareTo(const NumberID& other) const override
		{
			if (Value > other.Value) return CompareResult::Greater;
			if (Value < other.Value) return CompareResult::Less;
			return CompareResult::Equal;
		}

		[[nodiscard]] bool IsGreaterThan(const NumberID& other) const override
		{
			return Value > other.Value;
		}

		[[nodiscard]] bool IsLessThan(const NumberID& other) const override
		{
			return Value < other.Value;
		}

		[[nodiscard]] bool IsEqualTo(const NumberID& other) const override
		{
			return Value == other.Value;
		}
	};

	using NumberIDI8 = NumberID<std::int8_t>;
	using NumberIDU8 = NumberID<std::uint8_t>;
	using NumberIDI16 = NumberID<std::int16_t>;
	using NumberIDU16 = NumberID<std::uint16_t>;
	using NumberIDI32 = NumberID<std::int32_t>;
	using NumberIDU32 = NumberID<std::uint32_t>;
	using NumberIDI64 = NumberID<std::int64_t>;
	using NumberIDU64 = NumberID<std::uint64_t>;
}
