#include <LangYa/CodeLogic/Task.hpp>

namespace LangYa::CodeLogic
{
	Task::~Task()
	= default;

	void Task::Run()
	{
	}

	Task::operator std::function<void()>()
	{
		return [this] { Run(); };
	}

	Tasks& Tasks::operator+=(const FunctionType& function)
	{
		FunctionList.push_back(function);
		return *this;
	}

	Tasks& Tasks::operator+=(Task& task)
	{
		FunctionList.push_back(FunctionType(task));
		return *this;
	}

	void Tasks::Run()
	{
		for (auto& function : FunctionList)
		{
			function();
		}
	}

	void Tasks::Run(const std::atomic_bool& interruptSignal) const
	{
		for (auto& function : FunctionList)
		{
			if (interruptSignal) return;
			function();
		}
	}
}
