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

		Position2(const Position2& other) = default;
		Myself operator=(const Position2& other) = default;
		Position2(Position2&& other) = default;
		Myself operator=(Position2&& other) = default;
		~Position2() = default;
	};
}
