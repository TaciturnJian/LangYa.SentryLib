#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/ApplicationInfo.hpp>

namespace LangYa::SentryLib
{
	std::string
	ApplicationInfo
	::ToString() const
	{
		// 使用流保存字符串后输出
		// 相比于直接使用 fmt 效率更高。

		std::stringstream stream{};
		stream << *this;
		return stream.str();
	}

	std::ostream&
	operator<<(std::ostream& stream, const ApplicationInfo& info)
	{
		// 如初内容为以下格式的json文本（其中<Content>表示被替换的内容）
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
		// 使用 spdlog 格式化输出
		spdlog::info("Application: ");
		spdlog::info("  - Name: {}", Name);
		spdlog::info("  - Description: {}", Description);
		spdlog::info("  - Version: {}", Version.ToString());
	}
}
