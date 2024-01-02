#pragma once

#include <Sango/CompareResult.hpp>

namespace Sango
{
	/// @brief 代表能够与模板参数 T 比较的对象。
	///	@details 此接口代表对象能与 T 比较，结果由如下枚举表示：
	///	@code Sango::CompareResult @endcode
	///	提供了默认的单方面比较大于小于等于的方法，如果需要更好的性能，请重载它们。
	template <typename T>
	class IComparable
	{
	public:
		/// @brief 默认析构函数，防止子类无法正确释放。
		virtual ~IComparable() = default;

		/// @brief 比较此对象与 other 的大小。
		///	@details 重载此方法实现 Compare 功能，函数需要返回如下枚举：
		///	@code Sango::CompareResult @endcode
		///	@return A.CompareTo(B) 返回 A 比 B 怎样。
		[[nodiscard]] virtual CompareResult CompareTo(const T& other) const = 0;

		/// @brief 判断 A 是否大于 B 。
		[[nodiscard]] virtual bool IsGreaterThan(const T& other) const
		{
			return CompareTo(other) == CompareResult::Greater;
		}

		/// @brief 判断 A 是否小于 B 。
		[[nodiscard]] virtual bool IsLessThan(const T& other) const
		{
			return CompareTo(other) == CompareResult::Less;
		}

		/// @brief 判断 A 是否等于 B 。
		[[nodiscard]] virtual bool IsEqualTo(const T& other) const
		{
			return CompareTo(other) == CompareResult::Equal;
		}
	};
}
