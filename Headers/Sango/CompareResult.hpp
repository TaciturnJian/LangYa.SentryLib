#pragma once

namespace Sango
{
	/// @brief 表示比较的结果。
	///	@details 它是如下函数的返回值：
	///	@code
	///	CompareResult CompareAToB(const A& a, const B& b);
	///	@endcode
	enum class CompareResult : char
	{
		/// @brief 表示比较的结果是 A < B 命题为真
		Less = -1,

		/// @brief 表示比较的结果是 A == B 命题为真
		Equal = 0,

		/// @brief 表示比较的结果是 A > B 为真
		Greater = 1
	};

	/// @brief 如果比较的结果十分模糊，使用此枚举。
	///	@details 它是如下函数的返回值：
	///	@code
	///	CompareResultCode CompareAToB
	///	@endcode
	enum class CompareResultCode : char
	{
		None       = 0b00000000,
		Equal      = 0b00000001,
		Greater    = 0b00000010,
		Less       = 0b00000100,
		NotEqual   = Greater | Less,
		NotGreater = Equal | Less,
		NotLess    = Equal | Greater,
		All        = Equal | Greater | Less
	};
}
