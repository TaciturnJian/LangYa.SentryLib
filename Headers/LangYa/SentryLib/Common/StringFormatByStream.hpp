#pragma once

#include <ostream>

namespace LangYa::SentryLib
{
	class StringFormatByStream
	{
	public:
		virtual ~StringFormatByStream();

		virtual std::ostream& FormatToString(std::ostream& stream) const = 0;
	};

}
