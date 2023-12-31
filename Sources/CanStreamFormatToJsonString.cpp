#include <sstream>
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/StreamFormat/CanStreamFormatToJsonString.hpp>

namespace LangYa::SentryLib
{
	CanStreamFormatToJsonString
	::~CanStreamFormatToJsonString()
	= default;

	std::string
	FormatToJsonString(const CanStreamFormatToJsonString& obj)
	{
		std::stringstream stream{};
		stream << obj;
		return stream.str();
	}

	std::ostream& 
	operator<<(std::ostream& stream, const CanStreamFormatToJsonString& obj)
	{
		return obj.FormatToJsonString(stream);
	}

	std::ostream& 
	operator<<(std::ostream& stream, const CanStreamFormatToJsonString* objPtr)
	{
		if (objPtr == nullptr)
		{
			spdlog::warn("CanStreamFormatToJsonString> Invalid Attempt: trying to format a nullptr!");
			return stream;
		}

		return objPtr->FormatToJsonString(stream);
	}
}
