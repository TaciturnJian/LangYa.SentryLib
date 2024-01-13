#pragma once

namespace LangYa::CodeLogic 
{
	/// @brief Destination 代表一种函数 void(const TItem& item) 它往往用来接受数据。
	/// @tparam TItem 接受的数据类型。
	template <typename TItem>
	struct Destination
	{
		virtual ~Destination() = default;
		using ItemType = TItem;

		virtual void SetItem(const TItem& item)
		{
		}
	};
}
