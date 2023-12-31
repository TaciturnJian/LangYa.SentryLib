#include <iomanip>
#include <sstream>

#include <spdlog/spdlog.h>
#include <boost/json/parse.hpp>

#include <LangYa/SentryLib/Application/MonitorPackage.hpp>

namespace LangYa::SentryLib
{
	MonitorPackage::ItemType
	::ItemType() = default;

	MonitorPackage::ItemType
	::ItemType(std::string name, const long long value):
		Name{std::move(name)},
		Value{value}
	{
	}

	std::ostream&
	MonitorPackage::ItemType
	::FormatToConsole(std::ostream& stream) const
	{
		return stream << Name << ": " << std::setw(10) << Value;
	}

	std::ostream&
	MonitorPackage::ItemType
	::FormatToJson(std::ostream& stream) const
	{
		return stream << R"({"Name":")" << Name << R"(","Value":)" << Value << '}';
	}

	bool
	MonitorPackage
	::ParseJson(const std::string& jsonText)
	{
		try
		{
			auto obj = boost::json::parse(jsonText).as_object();

			Name = obj["Name"].as_string();

			for (auto items = obj["Items"].as_array(); auto& item : items)
			{
				auto item_obj = item.as_object();
				Items.emplace_back(item_obj["Name"].as_string().c_str(), item_obj["Value"].as_int64());
			}
			return true;
		}
		catch (std::exception& ex)
		{
			std::stringstream stream{};
			stream << "p_ex: " << ex.what();
			Items.emplace_back(stream.str(), 0);
		}
		catch (...)
		{
			Items.emplace_back("p_ex", 0);
		}
		return false;
	}

	std::ostream&
	MonitorPackage
	::FormatToJson(std::ostream& stream) const
	{
		stream << R"({"Name":")" << Name << R"(","Items":[)";
		const auto count = Items.size();
		for (unsigned long long i{0}; i < count - 1; i++)
		{
			Items[i].FormatToJson(stream) << ',';
		}
		return Items[count - 1].FormatToJson(stream) << "]}";
	}

	std::ostream&
	MonitorPackage
	::FormatToConsole(std::ostream& stream) const
	{
		stream << Name << ":\n";
		for (auto& item : Items)
		{
			item.FormatToConsole(stream << " - ") << '\n';
		}
		return stream << '\n';
	}

	void
	MonitorPackage
	::WriteJsonToSharedConnection(const std::shared_ptr<Connection>& connectionPtr) const
	{
		auto& connection = *connectionPtr;
		if (!connection.IsConnected()) return;

		std::stringstream stream{};
		FormatToJson(stream);
		const auto message = stream.str();

		try
		{
			connection.Write(MemoryView{message.c_str(), message.size()});
		}
		catch (std::exception& ex)
		{
			spdlog::error("MonitorPackage> WriteJsonToSharedConnection failed: {}", ex.what());
		}
		catch (...)
		{
			spdlog::error("MonitorPackage> WriteJsonToSharedConnection failed");
		}
	}
}
