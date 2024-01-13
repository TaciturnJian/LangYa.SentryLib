#pragma once

#include <atomic>
#include <vector>
#include <functional>

namespace LangYa::CodeLogic
{
	/// @brief Task 定义了一种空函数 void() 作为任务，即拿到这个任务后，只需要执行 Run 就可以完成此任务，中间的异常需要自己捕获。
	struct Task
	{
		using FunctionType = std::function<void()>;

		virtual ~Task();

		virtual void Run();

		explicit operator FunctionType();
	};

	/// @brief Tasks 是任务容器，内部包含了许多 Task，可以通过 += 添加 Task，通过 Run 执行所有的 Task。
	struct Tasks : Task
	{
		using FunctionListType = std::vector<FunctionType>;

		FunctionListType FunctionList{};

		Tasks() = default;

		explicit Tasks(FunctionType function) : FunctionList({std::move(function)})
		{
		}

		Tasks(const std::initializer_list<FunctionType> functions) : FunctionList(functions)
		{
		}

		Tasks& operator +=(const FunctionType& function);

		Tasks& operator +=(Task& task);

		void Run() override;

		void Run(const std::atomic_bool& interruptSignal) const;
	};
}
