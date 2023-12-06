#include <LangYa/SentryLib/ApplicationInfo.hpp>

std::string LangYa::SentryLib::ApplicationInfo::ToString() const
{
	std::stringstream stream{};
	stream << *this;
	return stream.str();
}

std::ostream& LangYa::SentryLib::operator<<(std::ostream& stream, const ApplicationInfo& info)
{
	return stream << R"({"Name":")" << info.Name << R"(","Description":")" << info.Description <<
		R"(","Version":)" << info.Version << R"("})";
}

// ReSharper disable once CppInconsistentNaming
void LangYa::SentryLib::ApplicationInfo::OutputTo_spdlog() const
{
	spdlog::info("Application: ");
	spdlog::info("  - Name: {}", Name);
	spdlog::info("  - Description: {}", Description);
	spdlog::info("  - Version: {}", Version.ToString());
}
