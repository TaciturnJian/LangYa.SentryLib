#pragma once

#include <vector>
#include <sstream>
#include <initializer_list>

#include <LangYa/SentryLib/Common/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief 一系列数字构成的大数字。
	///	形式类似 A.B.C.D... ，构造时使用 {A,B,C,D...} 。
	///	@tparam TBasicNumber 基础数字类型，如 int。
	template <typename TBasicNumber>
	struct SerialNumbers
	{
		/// @brief 储存各项数字。
		///	最后的顺序从左往右，按照 vector 的下标排序。
		std::vector<TBasicNumber> Numbers{};

		/// @brief 初始化一个空的序列数。
		///	注意，这里是真的什么也没有，而不是 0 。
		SerialNumbers() = default;

		/// @brief 使用初始化列表构造一个序列数。
		///	本质是构造一个 std::vector 。
		///	@param numbers 依次填入的数字。
		SerialNumbers(const std::initializer_list<TBasicNumber>& numbers) :
			Numbers(numbers)
		{
		}

		/// @brief 使用 vector 构造一个序列数。
		///	本质是构造一个 std::vector 。
		///	@param numbers 依次填入的数字。
		explicit SerialNumbers(const std::vector<TBasicNumber>& numbers) :
			Numbers(numbers)
		{
		}

		/// @brief 访问第几个数字。
		///	@param index 数字的下标。
		///	@exception std::out_of_range 下标越界。
		///	@return 对应下表数字的引用。
		TBasicNumber& operator[](MemoryView::SizeType index)
		{
			if (Numbers.size() <= index)
			{
				throw std::out_of_range("SerialNumbers> Access index is out of bound.");
			}

			return Numbers[index];
		}

		/// @brief 访问第几个数字，但是是常数类型。
		///	@param index 数字的下标。
		///	@exception std::out_of_range 下标越界。
		///	@return 对应下表数字的常量引用。
		const TBasicNumber& operator[](MemoryView::SizeType index) const
		{
			if (Numbers.size() <= index)
			{
				throw std::out_of_range("SerialNumbers> Access index is out of bound.");
			}

			return Numbers[index];
		}

		/// @brief 类似于流，添加一个数字到序列数中。
		/// @param number 要添加的数字，会被拷贝。
		///	@return 当前实例的引用。
		SerialNumbers& operator<<(const TBasicNumber& number)
		{
			Numbers.push_back(number);
			return *this;
		}

		/// @brief 格式化输出到流中。
		///	@param stream 输出流。
		///	@param numbers 要输出的序列数。
		///	@return 参数中输出流的引用。
		friend std::ostream& operator<<(std::ostream& stream, const SerialNumbers& numbers)
		{
			auto max_index = numbers.Numbers.size() - 1;
			for (MemoryView::SizeType i = 0; i < max_index; i++)
			{
				stream << numbers.Numbers[i] << ".";
			}
			return stream << numbers.Numbers[max_index];
		}

		/// @brief 格式化输出为字符串。
		///	@return 序列数的字符串形式。
		[[nodiscard]] std::string ToString() const
		{
			std::stringstream stream;
			stream << *this;
			return stream.str();
		}
	};
}
