#pragma once

#include <vector>
#include <functional>

#include <LangYa/CodeLogic/Source.hpp>
#include <LangYa/CodeLogic/Destination.hpp>
#include <LangYa/CodeLogic/Task.hpp>

namespace LangYa::CodeLogic 
{
	template <typename TArgument>
	struct Action final : Source<std::vector<std::function<void(TArgument)>>>, Destination<TArgument>
	{
		using FunctionType = std::function<void(TArgument)>;
		using FunctionListType = std::vector<FunctionType>;

		std::vector<FunctionType> FunctionList{};

		Action() = default;

		explicit Action(FunctionType function) : FunctionList({std::move(function)})
		{
		}

		Action(const std::initializer_list<FunctionType>& functions) : FunctionList(functions)
		{
		}

		Action& operator +=(const FunctionType& function)
		{
			FunctionList.push_back(function);
			return *this;
		}

		void Execute(TArgument argument) const
		{
			for (const auto& function : FunctionList)
			{
				function(argument);
			}
		}

		explicit operator FunctionType() const
		{
			return [this](TArgument argument) { Execute(argument); };
		}

		bool GetItem(FunctionListType& item) override
		{
			item = FunctionList;
			return !item.empty();
		}

		void SetItem(const TArgument& item) override
		{
			Execute(item);
		}
	};

	template <>
	struct Action<void> final : Tasks, Source<std::vector<std::function<void()>>>
	{
		using FunctionType = std::function<void()>;
		using FunctionListType = std::vector<FunctionType>;

		void Execute() const;

		explicit operator FunctionType() const;

		bool GetItem(FunctionListType& item) override;
	};
}
