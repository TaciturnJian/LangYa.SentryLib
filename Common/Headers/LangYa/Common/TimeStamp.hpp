#pragma once

#include <chrono>

namespace LangYa::Common
{
	/// @brief 代表通用的时间戳
	struct TimeStamp
	{
		using ClockType = std::chrono::steady_clock;
		using TimePointType = ClockType::time_point;

		TimePointType TimePoint{};

		TimeStamp();
		explicit TimeStamp(const TimePointType& timePoint);
	};
}