#pragma once

#include <atomic>
#include <vector>
#include <memory>

namespace LangYa::SentryLib
{
	/// @brief 代表一个堆栈，管理一组对象的生命周期，在对象销毁后腾出空间供新对象使用。
	template <typename TItem>
	class ManagedStack final
	{
		std::atomic_bool IsDisposed{false};

	public:
		using SharedAtomicSignal = std::shared_ptr<std::atomic_bool>; 

		struct ItemInfo
		{
			SharedAtomicSignal Available{std::make_shared<std::atomic_bool>(true)};
			std::shared_ptr<TItem> Item{nullptr};
		};

		std::vector<ItemInfo> ItemInfoList{};

		ItemInfo& GetAvailableItem()
		{
			if (IsDisposed)
			{
				throw std::exception(); //TODO 指定异常种类
			}

			for (auto& info : ItemInfoList)
			{
				if (info.Available)
				{
					return info;
				}
			}
			ItemInfoList.emplace_back();
			return ItemInfoList.back();
		}

		~ManagedStack()
		{
			IsDisposed = true;
			for (auto& info : ItemInfoList)
			{
				info.Available->store(false);
				info.Item.reset();
			}
		}
	};
}
