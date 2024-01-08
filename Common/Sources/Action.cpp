#include <LangYa/Common/Action.hpp>

namespace LangYa::Common
{
	Action<void>
	::Action()
	= default;

	Action<void>
	::Action(const std::initializer_list<ActionFunctionType> functions):
		FunctionList(functions)
	{
	}

	Action<void>
	::Action(FunctionListType functions):
		FunctionList(std::move(functions))
	{
	}

	void
	Action<void>
	::operator+=(const ActionFunctionType& right)
	{
		FunctionList.emplace_back(right);
	}

	void
	Action<void>
	::operator()() const
	{
		for (const auto& func : FunctionList)
		{
			func();
		}
	}

	Action<void>
	::operator std::vector<std::function<void()>>() const
	{
		return FunctionList;
	}

	Action<void>
	::operator std::function<void()>() const
	{
		return [this]()
		{
			(*this)();
		};
	}
}
