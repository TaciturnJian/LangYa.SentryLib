#pragma once

#include <LangYa/SentryLib/Math/Position1.hpp>

namespace LangYa::SentryLib
{
	template <typename TElement>
	struct Position2 : Position1<TElement>
	{
		TElement Y{0};

		Position2() = default;

		Position2(const TElement& x, const TElement& y) : Position1<TElement>{x}, Y{y}
		{
		}
	};

	using Position2C = Position2<char>;
	using Position2S = Position2<short>;
	using Position2I = Position2<int>;
	using Position2Ui = Position2<unsigned int>;
	using Position2F = Position2<float>;
	using Position2D = Position2<double>;
}
