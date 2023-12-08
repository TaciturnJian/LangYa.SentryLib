#pragma once

#include <vector>
#include <sstream>
#include <initializer_list>

#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一系列数字构成的大数字。
	///	形式类似 A.B.C.D... ，构造时使用 {A,B,C,D...} 。
	///	@tparam TBasicNumber 基础数字类型，如 int。
	template <typename TBasicNumber>
	struct SerialNumbers
	{
		std::vector<TBasicNumber> Numbers{};

		SerialNumbers() = default;

		SerialNumbers(const std::initializer_list<TBasicNumber>& numbers) :
			Numbers(numbers)
		{
		}

		explicit SerialNumbers(const std::vector<TBasicNumber>& numbers) :
			Numbers(numbers)
		{
		}

		SerialNumbers(const SerialNumbers& other) = default;

		SerialNumbers(SerialNumbers&& other) = default;

		SerialNumbers& operator=(const SerialNumbers& other) = default;

		SerialNumbers& operator=(SerialNumbers&& other) = default;

		TBasicNumber& operator[](MemoryView::SizeType index)
		{
			if (Numbers.size() <= index)
			{
				throw std::out_of_range("SerialNumbers> Access index is out of bound.");
			}

			return Numbers[index];
		}

		const TBasicNumber& operator[](MemoryView::SizeType index) const
		{
			if (Numbers.size() <= index)
			{
				throw std::out_of_range("SerialNumbers> Access index is out of bound.");
			}

			return Numbers[index];
		}

		friend std::ostream& operator<<(std::ostream& stream, const SerialNumbers& numbers)
		{
			auto max_index = numbers.Numbers.size() - 1;
			for (MemoryView::SizeType i = 0; i < max_index; i++)
			{
				stream << numbers.Numbers[i] << ".";
			}
			return stream << numbers.Numbers[max_index];
		}

		SerialNumbers& operator<<(const TBasicNumber& number)
		{
			Numbers.push_back(number);
			return *this;
		}

		[[nodiscard]] std::string ToString() const
		{
			std::stringstream stream;
			stream << *this;
			return stream.str();
		}
	};
}
