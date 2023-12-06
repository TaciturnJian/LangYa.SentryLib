#pragma once

#include <chrono>

namespace LangYa::SentryLib 
{
	/// @brief 时间戳
	struct TimeStamp
	{
		using ClockType = std::chrono::steady_clock;
		using TimePointType = ClockType::time_point;

		TimePointType TimePoint{};

		TimeStamp() = default;
		explicit TimeStamp(const TimePointType& timePoint) : TimePoint{ timePoint } {}
		TimeStamp(const TimeStamp& other) = default;
		TimeStamp(TimeStamp&& other) = default;
		TimeStamp& operator=(const TimeStamp& other) = default;
		TimeStamp& operator=(TimeStamp&& other) = default;
	};

	/// @brief 表明一个类型含有时间戳成员。
	///	其概念几乎等价于继承了 @code LangYa::SentryLib::TimeStamp @endcode。
	///	@tparam T 受检查的类型。
	template<typename T>
	concept TimeStamped = requires (T* p)
	{
		TimeStamp{p->TimePoint};
	};
}