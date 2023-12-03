#pragma once

#include <string>
#include <ostream>
#include <sstream>
#include <initializer_list>

#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib
{
	template <typename TElement, MemoryView::SizeType Dimension>
	class Vector
	{
	protected:
		TElement Element[Dimension]{0};

	public:
		using Myself = Vector&;

		static constexpr MemoryView::SizeType GetDimension()
		{
			return Dimension;
		}

		Vector() = default;

		Vector(Vector&& other) = default;
		Myself operator=(Vector&& other) = default;

		explicit Vector(const TElement& element)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] = element;
			}
		}

		Vector(const std::initializer_list<TElement>& list)
		{
			MemoryView::SizeType dimension = 0;
			for (const auto& element : list)
			{
				if (dimension >= Dimension) return;
				Element[dimension++] = element;
			}
		}

		Vector(const Vector& other) = default;

		~Vector() = default;

		Myself operator=(const TElement& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] = right;
			}
			return *this;
		}

		Myself operator=(const Vector& right) = default;

		Myself operator+=(const Vector& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] += right;
			}
			return *this;
		}

		friend Vector operator+(const Vector& left, const Vector& right)
		{
			Vector result{};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] + right[i];
			}
			return result;
		}

		Myself operator-=(const Vector& right)
		{
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				Element[i] -= right;
			}
			return *this;
		}

		friend Vector operator-(const Vector& left, const Vector& right)
		{
			Vector result{};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result[i] = left[i] - right[i];
			}
			return result;
		}

		friend TElement operator*(const Vector& left, const Vector& right)
		{
			TElement result{0};
			for (MemoryView::SizeType i = 0; i < Dimension; ++i)
			{
				result += left[i] * right[i];
			}
			return result;
		}

		friend std::ostream& operator<<(std::ostream& left, const Vector& right)
		{
			left << '(';
			for (MemoryView::SizeType i = 0; i < Dimension - 1; ++i)
			{
				left << right[i] << ',';
			}
			left << right[Dimension - 1] << ')';
			return left;
		}

		[[nodiscard]] std::string ToString() const
		{
			std::stringstream stream;
			stream << '(';
			for (MemoryView::SizeType i = 0; i < Dimension - 1; ++i)
			{
				stream << Element[i] << ',';
			}
			stream << Element[Dimension - 1] << ')';
			return stream.str();
		}

		TElement& operator[](const MemoryView::SizeType& dimension)
		{
			return Element[dimension];
		}

		const TElement& operator[](const MemoryView::SizeType& dimension) const
		{
			return Element[dimension];
		}
	};

	template<MemoryView::SizeType Dimension>
	using VectorI = Vector<int, Dimension>;

	template<MemoryView::SizeType Dimension>
	using VectorF = Vector<float, Dimension>;

	template<MemoryView::SizeType Dimension>
	using VectorD = Vector<double, Dimension>;
}
