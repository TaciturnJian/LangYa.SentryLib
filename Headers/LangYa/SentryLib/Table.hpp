#pragma once

#include <any>
#include <map>
#include <string>
#include <ostream>
#include <sstream>

namespace LangYa::SentryLib
{
	/// @brief Lua 中 table 数据类型的 C++ 实现。
	/// 提供一个类似于 Lua 中 table 的数据类型，可以存储任意类型的数据。
	///	至于能不能把其中的数据正确地取出来，得靠使用者了。
	class Table
	{
		/// @brief 用于存储数据的容器。
		std::map<std::string, std::any> Map{};

	public:
		template <typename T>
		T& Get(const std::string& key)
		{
			if (!Map.contains(key))
			{
				Map.insert(std::make_pair(key, std::make_any<T>()));
			}

			return std::any_cast<T&>(Map[key]);
		}

		template <typename T>
		void Get(const std::string& key, T& result)
		{
			result = Get<T>(key);
		}

		template <typename T>
		void Set(const std::string& key, const T& value)
		{
			Map.insert_or_assign(key, value);
		}

		std::any& operator[](const std::string& key)
		{
			return Map[key];
		}

		[[nodiscard]] std::string ToString() const
		{
			std::stringstream stream;
			stream << *this;
			return stream.str();
		}

		friend std::ostream& operator<<(std::ostream& stream, const Table& table)
		{
			stream << '{';

			unsigned long long index = 0;
			const unsigned long long max_index = table.Map.size() - 1;
			for (const auto& [key, value] : table.Map)
			{
				stream << "[\"" << key << "\"]=";

				if (const auto& v_type = value.type(); typeid(bool) == v_type)
					stream << std::any_cast<bool>(value);
				else if (typeid(char) == v_type) 
					stream << std::any_cast<char>(value);
				else if (typeid(unsigned char) == v_type) 
					stream << std::any_cast<unsigned char>(value);
				else if (typeid(short) == v_type) 
					stream << std::any_cast<short>(value);
				else if (typeid(unsigned short) == v_type) 
					stream << std::any_cast<unsigned short>(value);
				else if (typeid(int) == v_type)
					stream << std::any_cast<int>(value);
				else if (typeid(unsigned int) == v_type)
					stream << std::any_cast<unsigned int>(value);
				else if (typeid(float) == v_type) 
					stream << std::any_cast<float>(value);
				else if (typeid(double) == v_type) 
					stream << std::any_cast<double>(value);
				else if (typeid(long) == v_type) 
					stream << std::any_cast<long>(value);
				else if (typeid(unsigned long) == v_type) 
					stream << std::any_cast<unsigned long>(value);
				else if (typeid(long long) == v_type) 
					stream << std::any_cast<long long>(value);
				else if (typeid(unsigned long long) == v_type) 
					stream << std::any_cast<unsigned long long>(value);
				else if (typeid(std::string) == v_type) 
					stream << '"' << std::any_cast<std::string>(value) << '"';
				else stream << '"' << &value << '"';

				if (index++ < max_index)
				{
					stream << ',';
				}
			}

			return stream << '}';
		}
	};
}
