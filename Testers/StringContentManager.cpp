#include <chrono>
#include <iostream>
#include <format>
#include <LangYa/Common/Action.hpp>

class TaskExecutor
{
public:
	using TaskType = std::function<void()>;

	virtual ~TaskExecutor() = default;

	virtual void Execute(const TaskType& task)
	{
		task();
	}
};

int main()
{
	using namespace LangYa::Common;
}
