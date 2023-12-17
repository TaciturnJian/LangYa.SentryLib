#pragma once

#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <ostream>

#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>

#include <LangYa/SentryLib/Common/Connection.hpp>

namespace LangYa::SentryLib
{
	struct MonitorPackage
	{
		struct ItemType
		{
			std::string Name{"ItemName"};
			long long Value{0};

			ItemType() = default;

			ItemType(std::string name, const long long value) :
				Name{std::move(name)},
				Value{value}
			{
			}

			std::ostream& FormatToConsole(std::ostream& stream) const
			{
				return stream << Name << ": " << std::setw(10) << Value;
			}

			std::ostream& FormatToJson(std::ostream& stream) const
			{
				return stream << R"({"Name":")" << Name << R"(","Value":)" << Value << '}';
			}
		};

		std::string Name{"PackageName"};

		std::vector<ItemType> Items{};

		bool ParseJson(const std::string& jsonText)
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
				// TODO 处理错误
				std::stringstream stream{};
				stream << "p_ex: " << ex.what();
				Items.emplace_back(stream.str(), 0);
			}
			catch (...)
			{
				// TODO 未知的错误
				Items.emplace_back("p_ex", 0);
			}
			return false;
		}

		std::ostream& FormatToJson(std::ostream& stream) const
		{
			stream << R"({"Name":")" << Name << R"(","Items":[)";
			const auto count = Items.size();
			for (unsigned long long i{0}; i < count - 1; i++)
			{
				Items[i].FormatToJson(stream) << ',';
			}
			return Items[count - 1].FormatToJson(stream) << "]}";
		}

		std::ostream& FormatToConsole(std::ostream& stream) const
		{
			stream << Name << ":\n";
			for (auto& item : Items)
			{
				item.FormatToConsole(stream << " - ") << '\n';
			}
			return stream << '\n';
		}

		void WriteJsonToConnection(const std::shared_ptr<Connection>& connectionPtr) const
		{
			auto& connection = *connectionPtr;
			if (!connection.IsConnected()) return;

			std::stringstream stream{};
			FormatToJson(stream);
			const auto message = stream.str();

			try {
				connection.Write(MemoryView{message.c_str(), message.size()});
			}
			catch(...)
			{
				// TODO 处理异常
			}

		}
	};
}
