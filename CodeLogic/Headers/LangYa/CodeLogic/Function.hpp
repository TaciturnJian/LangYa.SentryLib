#pragma once

namespace LangYa::CodeLogic 
{
	/// @brief Function 定义了普通的函数 TResult(const TArgument&) 或 bool(const TArgument&, TResult&) 且不对参数数据做修改。
	/// @tparam TArgument 函数参数数据类型。
	/// @tparam TResult 函数结果数据类型。
	template <typename TArgument, typename TResult>
	struct Function
	{
		virtual ~Function() = default;
		using ArgumentType = TArgument;
		using ResultType = TResult;

		virtual bool Process(const TArgument& argument, TResult& result) { return false; }

		virtual TResult Process(const TArgument& argument)
		{
			TResult result{};
			return Process(argument, result) ? result : TResult{};
		}
	};
}
