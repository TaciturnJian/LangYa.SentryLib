#pragma once

#include <LangYa/SentryLib/Position1.hpp>

namespace LangYa::SentryLib
{
	template <typename TElement>
	struct Position2 : Position1<TElement>
	{
		TElement Y{0};

		using Myself = Position2&;
		using BasePosition = Position1<TElement>;
		Position2() = default;

		Position2(const TElement& x, const TElement& y) : BasePosition{x}, Y{y}
		{
		}

		~Position2() = default;
	};

	using Position2I = Position2<int>;
	using Position2Ui = Position2<unsigned int>;
	using Position2F = Position2<float>;
	using Position2D = Position2<double>;
}
