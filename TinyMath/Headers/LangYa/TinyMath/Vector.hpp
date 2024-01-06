#pragma once

#include <initializer_list>
#include <vector>

#include <LangYa/Common/MemoryView.hpp>

namespace LangYa::TinyMath
{
	template <typename TElement, Common::MemoryView::SizeType Dimension>
	class Vector final : public Common::IFormatByStream
	{
	protected:
		TElement Element[Dimension]{0};

	public:
		static constexpr Common::MemoryView::SizeType GetDimension()
		{
			return Dimension;
		}

		Vector() = default;

		explicit Vector(const TElement& element)
		{
			for (Common::MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] = element;
			}
		}

		Vector(const std::initializer_list<TElement>& list)
		{
			Common::MemoryView::SizeType dimension = 0;
			for (const auto& element : list)
			{
				if (dimension >= Dimension) return;
				Element[dimension++] = element;
			}
		}

		explicit Vector(const std::vector<TElement>& vector)
		{
			Common::MemoryView::SizeType dimension = 0;
			for (const auto& element : vector)
			{
				if (dimension >= Dimension) return;
				Element[dimension++] = element;
			}
		}

		Vector& operator=(const TElement& right)
		{
			for (Common::MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] = right;
			}
			return *this;
		}

		friend Vector operator+(const Vector& left, const Vector& right)
		{
			Vector result{};
			for (Common::MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] + right[i];
			}
			return result;
		}

		friend Vector operator-(const Vector& left, const Vector& right)
		{
			Vector result{};
			for (Common::MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] - right[i];
			}
			return result;
		}

		friend TElement operator*(const Vector& left, const Vector& right)
		{
			TElement result{0};
			for (Common::MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result += left[i] * right[i];
			}
			return result;
		}

		Vector& operator+=(const Vector& right)
		{
			for (Common::MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] += right[i];
			}
			return *this;
		}

		Vector& operator-=(const Vector& right)
		{
			for (Common::MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] -= right[i];
			}
			return *this;
		}

		TElement& operator[](const Common::MemoryView::SizeType& dimension)
		{
			return Element[dimension];
		}

		const TElement& operator[](const Common::MemoryView::SizeType& dimension) const
		{
			return Element[dimension];
		}

		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override
		{
			if (Dimension == 0)
			{
				return stream << "[]";
			}

			stream << '[';
			for (Common::MemoryView::SizeType i = 0; i < Dimension - 1; ++i)
			{
				stream << Element[i] << ',';
			}
			return stream << Element[Dimension - 1] << ']';
		}
	};

	template <typename TElement>
	using Vector2 = Vector<TElement, 2>;
	using Vector2C = Vector2<char>;
	using Vector2S = Vector2<short>;
	using Vector2I = Vector2<int>;
	using Vector2F = Vector2<float>;
	using Vector2D = Vector2<double>;

	template <typename TElement>
	using Vector3 = Vector<TElement, 3>;
	using Vector3C = Vector3<char>;
	using Vector3S = Vector3<short>;
	using Vector3I = Vector3<int>;
	using Vector3F = Vector3<float>;
	using Vector3D = Vector3<double>;

	/// @brief 叉乘运算符。
	/// @tparam TElement 向量的元素 。
	/// @param left 左边的向量（函数的第一个参数）。
	/// @param right 右边的向量（函数的第二个参数）。
	/// @return 叉乘的结果向量。
	template <typename TElement>
	Vector3<TElement> operator^(const Vector3<TElement>& left, const Vector3<TElement>& right)
	{
		const auto& x1 = left[0];
		const auto& y1 = left[1];
		const auto& z1 = left[2];

		const auto& x2 = right[0];
		const auto& y2 = right[1];
		const auto& z2 = right[2];

		return {
			y1 * z2 - z1 * y2,
			z1 * x2 - x1 * z2,
			x1 * y2 - y1 * x2
		};
	}
}
