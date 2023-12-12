#pragma once

#include <LangYa/SentryLib/Math/Position2.hpp>

namespace LangYa::SentryLib
{
	template <typename TElement>
	struct Position3 final : Position2<TElement>
	{
		TElement Z;

		Position3() = default;

		Position3(const TElement& x, const TElement& y, const TElement& z) : Position2<TElement>{x, y}, Z{z}
		{
		}
	};

	using Position3I = Position3<int>;
	using Position3F = Position3<float>;
	using Position3D = Position3<double>;
}
