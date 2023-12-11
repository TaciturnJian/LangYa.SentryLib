#pragma once

namespace LangYa::SentryLib {
	template<typename TItem, typename TKey>
	class Manager
	{
	public:
		using ItemType = TItem;
		using KeyType = TKey;

		KeyType AddItem(const TItem& item);
		KeyType SearchItem(const TItem& item);
		bool ContainsItem(const TItem& item);

	};
}