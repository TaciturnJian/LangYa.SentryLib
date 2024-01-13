#include <LangYa/CodeLogic/Action.hpp>

namespace Sango
{
	void Action<void>::Execute() const
	{
		for (const auto& function : FunctionList)
		{
			function();
		}
	}

	Action<void>::operator std::function<void()>() const
	{
		return [this] { Execute(); };
	}

	bool Action<void>::GetItem(FunctionListType& item)
	{
		item = FunctionList;
		return !item.empty();
	}
}
