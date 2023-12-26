#pragma once

#include <LangYa/SentryLib/CanStreamFormatToJson.hpp>

namespace LangYa::SentryLib
{
	/// @brief 使用一个元素 X 代表坐标。
	///	包装了一个元素，为了与其他 Position 保持一致。
	///	@tparam TElement 能够完全表示此位置的一个元素，此元素必须具有类似数字的功能，
	///	即提供等于赋值、加减乘除等四则运算等基本数字都有的功能。
	template <typename TElement>
	struct Position1 : CanStreamFormatToJson
	{
		/// @brief 位置的 X 坐标
		TElement X{};

		/// @brief 使用默认构造一维位置，其值为类型的默认值。
		Position1() = default;

		/// @brief 拷贝和移动构造一维位置。
		Position1(const Position1& other) : X{other.X} {}

		/// @brief 提供 x 坐标构建一维位置。
		explicit Position1(const TElement& x) : X{x} {}

		std::ostream& FormatToJson(std::ostream& stream) const override
		{
			return stream << R"({"X":)" << X << '}';
		}
	};

	using Position1C = Position1<char>;
	using Position1S = Position1<short>;
	using Position1I = Position1<int>;
	using Position1Ui = Position1<unsigned int>;
	using Position1F = Position1<float>;
	using Position1D = Position1<double>;
}
