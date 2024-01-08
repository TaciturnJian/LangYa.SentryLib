#include <LangYa/Common/TaskExecutor.hpp>

namespace LangYa::Common
{
	TaskExecutor::~TaskExecutor() = default;

	void TaskExecutor::Execute(const TaskType& task)
	{
		task();
	}
}
