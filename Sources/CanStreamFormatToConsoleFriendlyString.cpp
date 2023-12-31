#include <sstream>
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/StreamFormat/CanStreamFormatToConsoleFriendlyString.hpp>

namespace LangYa::SentryLib
{
	CanStreamFormatToConsoleFriendlyString
	::~CanStreamFormatToConsoleFriendlyString()
	= default;

	std::string
	FormatToConsoleFriendlyString(const CanStreamFormatToConsoleFriendlyString& obj)
	{
		std::stringstream stream{};
		stream << obj;
		return stream.str();
	}

	std::ostream&
	operator<<(std::ostream& stream, const CanStreamFormatToConsoleFriendlyString& obj)
	{
		return obj.FormatToConsoleFriendlyString(stream);
	}

	std::ostream&
	operator<<(std::ostream& stream, const CanStreamFormatToConsoleFriendlyString* objPtr)
	{
		if (objPtr == nullptr)
		{
			spdlog::warn("FormatToConsoleFriendlyString> Invalid Attempt: trying to format a nullptr!");
			return stream;
		}

		return objPtr->FormatToConsoleFriendlyString(stream);
	}
}
