#pragma once

#include <LangYa/SentryLib/Position.hpp>
#include <LangYa/SentryLib/Vector.hpp>

namespace LangYa::SentryLib {
	template<typename TElement>
	struct PositionedVector3 {
		Position3<TElement> Position{};
		Vector3<TElement> Direction{};

		friend Vector3<TElement> operator-(const PositionedVector3& left, const Vector3<TElement>& right)
		{
			return left.Direction - right;
		}

		friend Vector3<TElement> operator-(const Vector3<TElement>& left, const PositionedVector3& right)
		{
			return left - right.Direction;
		}

		friend Vector3<TElement> operator+(const PositionedVector3& left, const Vector3<TElement>& right)
		{
			return left.Direction + right;
		}

		friend Vector3<TElement> operator+(const Vector3<TElement>& left, const PositionedVector3& right)
		{
			return left + right.Direction;
		}

		PositionedVector3& operator+=(const Position3<TElement>& position)
		{
			Position += position;
			return *this;
		}

		PositionedVector3& operator+=(const Vector3<TElement>& direction)
		{
			Direction += direction;
			return *this;
		}

		PositionedVector3& operator=(const Position3<TElement>& position)
		{
			Position = position;
			return *this;
		}

		PositionedVector3& operator=(const Vector3<TElement>& direction)
		{
			Direction = direction;
			return *this;
		}
	};

	using PositionedVector3I = PositionedVector3<int>;
	using PositionedVector3F = PositionedVector3<float>;
	using PositionedVector3D = PositionedVector3<double>;

	template<typename TElement>
	struct PositionedVector2 {
		Position2<TElement> Position{};
		Vector2<TElement> Direction{};

		friend Vector2<TElement> operator-(const PositionedVector2& left, const Vector2<TElement>& right)
		{
			return left.Direction - right;
		}

		friend Vector2<TElement> operator-(const Vector2<TElement>& left, const PositionedVector2& right)
		{
			return left - right.Direction;
		}

		friend Vector2<TElement> operator+(const PositionedVector2& left, const Vector2<TElement>& right)
		{
			return left.Direction + right;
		}

		friend Vector2<TElement> operator+(const Vector2<TElement>& left, const PositionedVector2& right)
		{
			return left + right.Direction;
		}

		PositionedVector2& operator+=(const Position2<TElement>& position)
		{
			Position += position;
			return *this;
		}

		PositionedVector2& operator+=(const Vector2<TElement>& direction)
		{
			Direction += direction;
			return *this;
		}

		PositionedVector2& operator=(const Position2<TElement>& position)
		{
			Position = position;
			return *this;
		}

		PositionedVector2& operator=(const Vector2<TElement>& direction)
		{
			Direction = direction;
			return *this;
		}
	};

	using PositionedVector2I = PositionedVector2<int>;
	using PositionedVector2F = PositionedVector2<float>;
	using PositionedVector2D = PositionedVector2<double>;
}