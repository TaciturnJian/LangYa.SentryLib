#include <LangYa/Common/Action.hpp>

namespace LangYa::Common
{
	Action<>
	::Action()
	= default;

	Action<>
	::Action(const std::initializer_list<ActionFunctionType> functions):
		FunctionList(functions)
	{
	}

	Action<>
	::Action(FunctionListType functions):
		FunctionList(std::move(functions))
	{
	}

	void
	Action<>
	::operator+=(const ActionFunctionType& right)
	{
		FunctionList.emplace_back(right);
	}

	void
	Action<>
	::operator()() const
	{
		for (const auto& func : FunctionList)
		{
			func();
		}
	}

	Action<>
	::operator std::vector<std::function<void()>>() const
	{
		return FunctionList;
	}

	Action<>
	::operator std::function<void()>() const
	{
		return [this]()
		{
			(*this)();
		};
	}
}
