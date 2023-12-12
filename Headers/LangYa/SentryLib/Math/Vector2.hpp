#pragma once

#include <LangYa/SentryLib/Math/Vector.hpp>
#include <LangYa/SentryLib/Math/Position2.hpp>

namespace LangYa::SentryLib
{
	template <typename TElement, MemoryView::SizeType Dimension = 2>
	class Vector2 : public Vector<TElement, Dimension>
	{
	public:
		using BaseVector = Vector<TElement, Dimension>;
		using Myself = Vector2&;

		static constexpr MemoryView::SizeType GetDimension()
		{
			return Dimension;
		}

#pragma region Constructors & Deconstructors & Assign Operators

		Vector2() = default;

		explicit Vector2(const TElement& element) : BaseVector{element}
		{
		}

		Vector2(const std::initializer_list<TElement>& list) : BaseVector{list}
		{
		}

		Vector2(const Vector2& other) = default;

		explicit Vector2(const BaseVector& other) : BaseVector{other}
		{
		}

		explicit Vector2(const Position2<TElement>& position) : BaseVector{position.X, position.Y}
		{
		}

		Vector2(Vector2&& other) = default;
		Myself operator=(Vector2&& other) = default;

		~Vector2() = default;

		/// @brief Reset every element to the value.
		/// @param right The value to set to.
		///	@return Myself.
		Myself operator=(const TElement& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				BaseVector::Element[i] = right;
			}
			return *this;
		}

		/// @brief Basic copy assignment.
		///	@param right The vector to copy from.
		///	@return Myself.
		Myself operator=(const Vector2& right) = default;

#pragma endregion

#pragma region Operators

		Myself operator+=(const Vector2& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				BaseVector::Element[i] += right;
			}
			return *this;
		}

		friend Vector2 operator+(const Vector2& left, const Vector2& right)
		{
			Vector2 result{};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] + right[i];
			}
			return result;
		}

		Myself operator-=(const Vector2& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				BaseVector::Element[i] -= right;
			}
			return *this;
		}

		friend Vector2 operator-(const Vector2& left, const Vector2& right)
		{
			Vector2 result{};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] - right[i];
			}
			return result;
		}

		friend TElement operator*(const Vector2& left, const Vector2& right)
		{
			TElement result{0};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result += left[i] * right[i];
			}
			return result;
		}

		friend std::ostream& operator<<(std::ostream& left, const Vector2& right)
		{
			left << '(';
			for (TElement i = 0; i < Dimension - 1; ++i)
			{
				left << right[i] << ',';
			}
			left << right[Dimension - 1] << ')';
			return left;
		}

#pragma endregion

#pragma region Accessibility

		TElement& operator[](const MemoryView::SizeType& dimension)
		{
			return BaseVector::Element[dimension];
		}

		const TElement& operator[](const MemoryView::SizeType& dimension) const
		{
			return BaseVector::Element[dimension];
		}

		TElement& X() { return BaseVector::Element[0]; }
		[[nodiscard]] const TElement& X() const { return BaseVector::Element[0]; }

		TElement& Y() { return BaseVector::Element[1]; }
		[[nodiscard]] const TElement& Y() const { return BaseVector::Element[1]; }
#pragma endregion
	};

	using Vector2C = Vector2<char>;
	using Vector2S = Vector2<short>;
	using Vector2I = Vector2<int>;
	using Vector2F = Vector2<float>;
	using Vector2D = Vector2<double>;
}
