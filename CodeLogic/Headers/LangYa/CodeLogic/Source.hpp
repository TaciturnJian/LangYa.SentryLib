#pragma once

namespace LangYa::CodeLogic 
{
	/// @brief Source 定义了一种函数 TItem() 或 bool(TItem&) ，它们往往用来获取数据。
	/// @tparam TItem 获取的数据的类型。
	template<typename TItem>
	struct Source 
	{
		using ItemType = TItem;
		virtual ~Source() = default;
		virtual bool GetItem(ItemType& item) {return false;}
		virtual ItemType GetItem() {
			ItemType item{};
			return GetItem(item) ? item : ItemType{};
		}
	};
}
