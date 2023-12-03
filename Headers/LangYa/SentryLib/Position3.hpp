#pragma once

#include <LangYa/SentryLib/Position2.hpp>

namespace LangYa::SentryLib
{
	template <typename TElement>
	struct Position3 : Position2<TElement>
	{
		TElement Z;

		using Myself = Position3&;
		using BasePosition = Position2<TElement>;
		Position3() = default;

		Position3(const TElement& x, const TElement& y, const TElement& z) : BasePosition{x, y}, Z{z}
		{
		}

		Position3(const Position3& other) = default;
		Myself operator=(const Position3& other) = default;
		Position3(Position3&& other) = default;
		Myself operator=(Position3&& other) = default;
		~Position3() = default;
	};

	using Position3I = Position3<int>;
	using Position3F = Position3<float>;
	using Position3D = Position3<double>;
}
