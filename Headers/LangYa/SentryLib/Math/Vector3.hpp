#pragma once

#include <LangYa/SentryLib/Math/Vector.hpp>
#include <LangYa/SentryLib/Math/Position3.hpp>

namespace LangYa::SentryLib
{
	template <typename TElement, MemoryView::SizeType Dimension = 3>
	class Vector3 : public Vector<TElement, Dimension>
	{
	public:
		using BaseVector = Vector<TElement, Dimension>;
		using Myself = Vector3&;

		static constexpr MemoryView::SizeType GetDimension()
		{
			return Dimension;
		}

#pragma region Constructors & Deconstructors & Assign Operators

		Vector3() = default;

		explicit Vector3(const TElement& element) : BaseVector{element}
		{
		}

		Vector3(const std::initializer_list<TElement>& list) : BaseVector{list}
		{
		}

		Vector3(const Vector3& other) = default;
		Vector3(Vector3&& other) = default;
		Myself operator=(Vector3&& other) = default;

		explicit Vector3(const BaseVector& other) : BaseVector{other}
		{
		}

		explicit Vector3(const Position3<TElement>& position) : BaseVector{
			position.X, position.Y, position.Z
		}
		{
		}

		~Vector3() = default;

		Myself operator=(const TElement& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				BaseVector::Element[i] = right;
			}
			return *this;
		}

		Myself operator=(const Vector3& right) = default;

#pragma endregion

#pragma region Operators

		Myself operator+=(const Vector3& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				BaseVector::Element[i] += right;
			}
			return *this;
		}

		friend Vector3 operator+(const Vector3& left, const Vector3& right)
		{
			Vector3 result{};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] + right[i];
			}
			return result;
		}

		Myself operator-=(const Vector3& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				BaseVector::Element[i] -= right;
			}
			return *this;
		}

		friend Vector3 operator^(const Vector3& left, const Vector3& right)
		{
			return {
				left.Y() * right.Z() - left.Z() * right.Y(),
				left.Z() * right.X() - left.X() * right.Z(),
				left.X() * right.Y() - left.Y() * right.X()
			};
		}

		friend Vector3 operator-(const Vector3& left, const Vector3& right)
		{
			Vector3 result{};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] - right[i];
			}
			return result;
		}

		friend TElement operator*(const Vector3& left, const Vector3& right)
		{
			TElement result{0};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result += left[i] * right[i];
			}
			return result;
		}

		friend std::ostream& operator<<(std::ostream& left, const Vector3& right)
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

		TElement& Z() { return BaseVector::Element[2]; }
		[[nodiscard]] const TElement& Z() const { return BaseVector::Element[2]; }

#pragma endregion
	};

	using Vector3I = Vector3<int>;
	using Vector3F = Vector3<float>;
	using Vector3D = Vector3<double>;

}
