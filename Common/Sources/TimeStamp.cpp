#include <LangYa/Common/TimeStamp.hpp>

namespace LangYa::Common
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
