#pragma once

#include <vector>
#include <functional>

namespace LangYa::Common
{
	template <typename TArgument>
	class Action final
	{
	public:
		using ActionFunctionType = std::function<void(TArgument)>;
		using FunctionListType = std::vector<ActionFunctionType>;

	private:
		FunctionListType FunctionList{};

	public:
		Action() = default;

		Action(const std::initializer_list<ActionFunctionType> functions) :
			FunctionList(functions)
		{
		}

		explicit Action(FunctionListType functions) :
			FunctionList(std::move(functions))
		{
		}

		Action& operator+=(const ActionFunctionType& right)
		{
			FunctionList.emplace_back(right);
			return *this;
		}

		void operator()(TArgument args) const
		{
			for (const auto& func : FunctionList)
			{
				func(args);
			}
		}

		explicit operator FunctionListType() const
		{
			return FunctionList;
		}

		explicit operator ActionFunctionType() const
		{
			return [this](TArgument args)
			{
				(*this)(args);
			};
		}
	};

	template <>
	class Action<void> final
	{
	public:
		using ActionFunctionType = std::function<void()>;
		using FunctionListType = std::vector<ActionFunctionType>;

	private:
		FunctionListType FunctionList{};

	public:
		Action();

		Action(std::initializer_list<ActionFunctionType> functions);

		explicit Action(FunctionListType functions);

		void operator+=(const ActionFunctionType& right);

		void operator()() const;

		explicit operator FunctionListType() const;

		explicit operator ActionFunctionType() const;
	};
}
