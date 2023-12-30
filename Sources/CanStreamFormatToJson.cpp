#include <LangYa/SentryLib/CanStreamFormatToJson.hpp>

namespace LangYa::SentryLib
{
	CanStreamFormatToJson::~CanStreamFormatToJson() = default;

	std::string FormatToString(const CanStreamFormatToJson& streamFormat)
	{
		std::stringstream stream{};
		stream << streamFormat;
		return stream.str();
	}

	std::ostream& operator<<(std::ostream& stream, const CanStreamFormatToJson& obj)
	{
		return obj.FormatToJson(stream);
	}

	std::ostream& operator<<(std::ostream& stream, const CanStreamFormatToJson* objPtr)
	{
		if (objPtr == nullptr)
		{
			spdlog::warn("CanStreamFormatToJson> Invalid Attempt: trying to format a nullptr!");
			return stream;
		}

		return objPtr->FormatToJson(stream);
	}
}
