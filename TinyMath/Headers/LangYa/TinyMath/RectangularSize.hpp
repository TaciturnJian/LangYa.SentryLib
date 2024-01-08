#pragma once

#include <cstdint>

#include <spdlog/spdlog.h>

#include <LangYa/Common/IFormatByStream.hpp>

namespace LangYa::TinyMath {
	/// @brief 代表矩形的大小（无偏移点）。
	/// @tparam TValue 值的类型。
	template<typename TValue>
	struct RectangularSize final : Common::IFormatByStream
	{
		/// @brief 矩形的宽，或者矩形的下边界的长度（单位需要自行确定）。
		TValue Width{0};

		/// @brief 矩形的高，或者矩形的右边界的长度（单位需要自行确定）。
		TValue Height{0};

		RectangularSize() = default;

		RectangularSize(const TValue& width, const TValue& height) : Width(width), Height(height)
		{
		}

		TValue GetSize() const
		{
			return Width * Height;
		}

		friend bool operator==(const RectangularSize& left, const RectangularSize& right)
		{
			return right.Width - left.Width < 0.001f && right.Height - left.Height < 0.001f;
		}

		std::ostream& FormatByStream(std::ostream& stream, const int option = 0) const override
		{
			if (option == 0)	// 控制台友好型，输出两个尺寸，用空格隔开
			{
				return stream << Width << ' ' << Height;
			}

			if (option == 1)	// json 紧凑格式，输出数组
			{
				return stream << '[' << Width << ',' << Height << ']';
			}

			spdlog::warn("RectangularSize> Unknown option for formatting this size, trying with default option");

			return FormatByStream(stream);
		}
	};

	using RectangularSizeI = RectangularSize<int>;
	using RectangularSizeU8 = RectangularSize<std::uint8_t>;
	using RectangularSizeU16 = RectangularSize<std::uint16_t>;
	using RectangularSizeU32 = RectangularSize<std::uint32_t>;
	using RectangularSizeU64 = RectangularSize<std::uint64_t>;
	using RectangularSizeF = RectangularSize<float>;
	using RectangularSizeD = RectangularSize<double>;

}