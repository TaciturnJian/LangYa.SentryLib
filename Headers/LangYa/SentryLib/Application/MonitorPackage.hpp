#pragma once

#include <vector>
#include <string>

#include <LangYa/SentryLib/Common/Connection.hpp>

namespace LangYa::SentryLib
{
	/// @brief 监视器包裹，提供格式化
	struct MonitorPackage
	{
		/// @brief 监视器的单个物品。
		/// 定义了监视器的基本物品格式，可以转换为Json格式和控制台格式。
		///	因为实在找不到合适的名字所以放在了 Monitor 里面。
		///	实际使用起来对此类型几乎无感。
		struct ItemType
		{
			/// @brief 物品的名称。
			/// 在显示器上，显示的物品名称，最好不要包括特殊字符或中文字符。
			std::string Name{"ItemName"};

			/// @brief 物品的数值。
			///	在显示器上显示的物品数值，注意只能是 long long 类型。
			long long Value{0};

			/// @brief 默认构造函数。
			///	指定名称为 ItemName ，值为 0 。
			ItemType();

			/// @brief 给定参数构造物品。
			///	@param name 物品名称，最好不要包括特殊字符或中文字符。
			///	@param value 物品值，是一个 long long。
			ItemType(std::string name, long long value);

			/// @brief 使用流将物品转换为在屏幕上打印的格式。
			///	格式为 Name: Value(长度为10) 。
			///	末尾没有换行符。
			///	@param stream 流对象。
			///	@return 参数中传入的流对象。
			std::ostream& FormatToConsole(std::ostream& stream) const;

			/// @brief 使用流将物品转换为 Json 格式。
			///	格式为 {"Name":"<ItemName>","Value":"<ItemValue>"} 。
			///	末尾没有换行符。
			///	@param stream 流对象。
			///	@return 参数中传入的流对象。
			std::ostream& FormatToJson(std::ostream& stream) const;
		};

		/// @brief 整个包的名称。
		std::string Name{"PackageName"};

		/// @brief 整个包的物品。
		std::vector<ItemType> Items{};

		/// @brief 从 json 文本中解析出包。
		///	@param jsonText json 文本，需要保证有效，如果无效则返回 false 。
		///	@return 是否完全成功解析 json 并且此 json 格式与此包裹的格式相符合。
		///	详细格式见：
		///	@code LangYa::SentryLib::FormatToJsonString @endcode
		[[nodiscard]] bool ParseJson(const std::string& jsonText);

		/// @brief 将此包转换为 json 格式。
		///	此格式大致为：{"Name":"<PackageName>","Items":[{"Name":"<ItemName>","Value":"<ItemValue>"},...]}
		///	@param stream 流对象。
		///	@return 参数中传入的流对象，已经向其中写入了 json 格式的包。
		std::ostream& FormatToJson(std::ostream& stream) const;

		/// @brief 将此包转换为控制台格式。
		///	此格式大致为：
		///	PackageName:
		///	 - ItemName1: Value1(长度为10)
		///	 - ItemName2: Value2(长度为10)
		///	@param stream 流对象。
		///	@return 参数中传入的流对象，已经向其中写入了控制台格式的包。
		std::ostream& FormatToConsole(std::ostream& stream) const;

		/// @brief 将包格式化为 json 文本，然后将文本写入到连接中。
		///	@param connectionPtr 连接的共享指针，调用方应该确定此连接有效，且当前仅被此函数占用。
		void WriteJsonToSharedConnection(const std::shared_ptr<Connection>& connectionPtr) const;
	};
}
