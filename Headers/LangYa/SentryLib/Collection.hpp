#pragma once

#include <LangYa/SentryLib/Common/CSharpp.hpp>

namespace LangYa::SentryLib
{

	/// 我也不知道为什么这个会出现在这里，这不应该是语言层面的接口吗？
	template<typename TItem>
	SC_INTERFACE Collection {
	public:
		using ItemType = TItem;

		virtual ~Collection() = default;
		virtual void AddItem(const ItemType& item) = 0;
		virtual void RemoveItem(const ItemType& item) = 0;
		virtual bool ContainItem(const ItemType& item) = 0;
	};


}