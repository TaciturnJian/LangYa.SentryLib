#include <sstream>

#include <LangYa/SentryLib/IFormatByStream.hpp>

namespace LangYa::SentryLib
{
	IFormatByStream
	::~IFormatByStream()
	= default;

	std::string
	FormatToString(const IFormatByStream& obj, const int option)
	{
		std::stringstream stream{};
		obj.FormatByStream(stream, option);
		return stream.str();
	}

	std::ostream&
	operator<<(std::ostream& stream, const IFormatByStream& obj)
	{
		return obj.FormatByStream(stream);
	}
}
