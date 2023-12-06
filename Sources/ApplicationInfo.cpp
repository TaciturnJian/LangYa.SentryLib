#include <LangYa/SentryLib/ApplicationInfo.hpp>

namespace LangYa::SentryLib
{
	std::string
	ApplicationInfo
	::ToString() const
	{
		std::stringstream stream{};
		stream << *this;
		return stream.str();
	}

	std::ostream&
	operator<<(std::ostream& stream, const ApplicationInfo& info)
	{
		/*{"Name":"<Name>","Description":"<Description>","Version":"<Version>"}*/
		return stream
			<< R"({"Name":")"
			<< info.Name
			<< R"(","Description":")"
			<< info.Description
			<< R"(","Version":)"
			<< info.Version
			<< R"("})";
	}

	// ReSharper disable once CppInconsistentNaming
	void ApplicationInfo::OutputTo_spdlog() const
	{
		spdlog::info("Application: ");
		spdlog::info("  - Name: {}", Name);
		spdlog::info("  - Description: {}", Description);
		spdlog::info("  - Version: {}", Version.ToString());
	}
}
