#pragma once

#include <functional>

namespace LangYa::Common
{
	class TaskExecutor
	{
	public:
		using TaskType = std::function<void()>;

		virtual ~TaskExecutor();

		virtual void Execute(const TaskType& task);
	};
}
