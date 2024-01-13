#include <chrono>
#include <iostream>
#include <format>
#include <LangYa/Common/Action.hpp>
#include <Sango/CommonMember.hpp>
#include <Sango/Combine.hpp>

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
	using namespace Sango;

	using combine_result = Combine<
		CommonMember::HasName<std::string>,
		CommonMember::HasID<int>,
		CommonMember::HasInfo<float>
	>;

	combine_result comb{
		{
			""
		},
		1,
		0.3f
	};

	using namespace LangYa::Common;
}
