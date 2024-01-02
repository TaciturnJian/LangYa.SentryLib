#include <LangYa/SentryLib/Common/TimeStamp.hpp>

namespace LangYa::SentryLib
{
	TimeStamp
	::TimeStamp()
	= default;

	TimeStamp
	::TimeStamp(const TimePointType& timePoint):
		TimePoint{timePoint}
	{
	}
}
