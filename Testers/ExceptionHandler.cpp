// 这个文件主要用于测试各种处理异常的方法
// 简单来说有以下几种方式：
// 触发变量
// 记录日志
// 再进一步无法处理时终止程序

// 程序开始时需要管理资源
// 程序运行时需要在单线程中使用资源，多线程中防止竞争
// 程序结束时需要释放资源

#include <functional>
#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <thread>
#include <spdlog/spdlog.h>

using RawActionFunction = void();
using RawActionFunctionPointer = RawActionFunction*;
using RawActionFunctionPointerList = RawActionFunction*;
using ActionFunction = std::function<RawActionFunction>;
using ActionFunctionList = std::vector<ActionFunction>;
using MaxTimeCost = unsigned long long;

void EmptyFunction()
{
}

class NamedItem
{
public:
	bool IsEmpty{true};
	std::string Name{"<ItemName>"};

	NamedItem() = default;

	explicit NamedItem(const bool isEmpty, std::string name) :
		IsEmpty(isEmpty),
		Name(std::move(name))
	{
	}
};

template <typename T>
concept TNamedItem = requires(T* t, NamedItem* nItem)
{
	nItem = t;
};

using Index = unsigned long long;

template <TNamedItem T>
class Manager
{
protected:
	std::vector<T> Items{};

	Index GetIndexOfFirstEmptyItem()
	{
		Index index = 0;
		for (auto& item : Items)
		{
			if (item.IsEmpty)
			{
				return index;
			}
			++index;
		}
		Items.emplace_back({});
		return index;
	}

	T& GetFirstEmptyItem()
	{
		return Items[GetIndexOfFirstEmptyItem()];
	}

public:
	Manager() = default;

	explicit Manager(const std::vector<T>& items) :
		Items(items)
	{
	}

	Manager(std::initializer_list<T>& items) :
		Items(items)
	{
	}

	void Iterate(const std::function<void(T&)>& processor)
	{
		for (auto& item : Items)
		{
			if (item.IsEmpty) continue;
			processor(item);
		}
	}

	void Iterate(const std::function<void(const T&)>& processor) const
	{
		for (const auto& item : Items)
		{
			if (item.IsEmpty) continue;
			processor(item);
		}
	}

	void InterruptableIterate(const std::function<bool(T&)>& processor)
	{
		for (auto& item : Items)
		{
			if (item.IsEmpty) continue;

			if (processor(item))
			{
				return;
			}
		}
	}

	void InterruptableIterate(const std::function<bool(const T&)>& processor) const
	{
		for (const auto& item : Items)
		{
			if (item.IsEmpty) continue;

			if (processor(item))
			{
				return;
			}
		}
	}

	void Append(const T& item)
	{
		GetFirstEmptyItem() = item;
	}

	bool Remove(Index index)
	{
		if (index >= Items.size()) return false;
		auto& item = Items[index];
		if (item.IsEmpty) return false;
		item.IsEmpty = true;
		return true;
	}

	bool ContainIndex(Index index)
	{
		return index < Items.size() && Items[index].IsEmpty == false;
	}

	bool ContainItem(const T& item)
	{
		for (auto& storage_item : Items)
		{
			if (item == Items) return true;
		}

		return false;
	}
};

struct ActionInfo : NamedItem
{
	ActionFunction Function{EmptyFunction};
	MaxTimeCost TimeCost{0};

	ActionInfo() : NamedItem(true, "<ActionName>")
	{
	}

	ActionInfo(std::string name, ActionFunction function, const MaxTimeCost timeCost = 0) :
		NamedItem(false, std::move(name)),
		Function(std::move(function)),
		TimeCost(timeCost)
	{
	}
};

class Action
{
public:
	virtual ~Action() = default;

	[[nodiscard]] virtual const ActionInfo& GetActionInfo() const = 0;

	virtual void Execute() const
	{
		GetActionInfo().Function();
	}

	[[nodiscard]] virtual bool TryExecute() const
	{
		bool no_exception{true};

		try
		{
			Execute();
		}
		catch (...)
		{
			no_exception = false;
		}

		return no_exception;
	}

	[[nodiscard]] virtual bool TryExecute(std::ostream& stream) const
	{
		bool no_exception{true};
		const char* exception_str{nullptr};
		try
		{
			Execute();
		}
		catch (std::exception& ex)
		{
			no_exception = false;
			exception_str = ex.what();
		}
		catch (const char* ex)
		{
			no_exception = false;
			exception_str = ex;
		}
		catch (...)
		{
			no_exception = false;
			exception_str = " Unknown exception";
		}

		if (!no_exception)
		{
			stream << "Failed to execute action(" << GetActionInfo().Name << "): " << exception_str << '\n';
		}

		return no_exception;
	}

	const ActionInfo* operator->() const
	{
		return &GetActionInfo();
	}
};

class SingleAction : public Action
{
protected:
	ActionInfo Info{};

public:
	SingleAction() = default;

	explicit SingleAction(ActionInfo info) : Info(std::move(info))
	{
	}

	[[nodiscard]] const ActionInfo& GetActionInfo() const override
	{
		return Info;
	}
};

class ActionGroup final : public SingleAction
{
public:
	using FunctionExecuteIndex = unsigned long long;


private:
	std::vector<SingleAction> Actions{};
	bool Parallel{false};

	[[nodiscard]] auto GetInfoLambda() const
	{
		return [this]
		{
		};
	}

public:
	ActionGroup() = default;

	explicit ActionGroup(const bool parallel = false) :
		Parallel(parallel)
	{
	}

	ActionGroup(std::string groupName, const bool parallel, const std::vector<ActionInfo>& actions) :
		SingleAction(ActionInfo{std::move(groupName), [this] { Execute(); }}),
		Parallel(parallel)
	{
		for (auto& action : actions)
		{
			Actions.emplace_back(action);
		}
	}

	explicit ActionGroup(std::string groupName, const bool parallel, const std::initializer_list<ActionInfo> actions) :
		SingleAction(ActionInfo{std::move(groupName), [this] { Execute(); }}),
		Parallel(parallel)
	{
		for (auto& action : actions)
		{
			Actions.emplace_back(action);
		}
	}

	ActionGroup(std::string groupName, const bool parallel, const std::vector<SingleAction>& actions) :
		SingleAction(ActionInfo{std::move(groupName), GetInfoLambda()}),
		Actions(actions),
		Parallel(parallel)
	{
	}

	explicit ActionGroup(std::string groupName, const bool parallel,
	                     const std::initializer_list<SingleAction>& actions) :
		SingleAction(ActionInfo{std::move(groupName), GetInfoLambda()}),
		Actions(actions),
		Parallel(parallel)
	{
	}

	[[nodiscard]] const ActionInfo& GetActionInfo() const override
	{
		return Info;
	}

	void Execute() const override
	{
		if (!Parallel)
		{
			for (auto& action : Actions)
			{
				action.Execute();
			}
			return;
		}

		std::vector<std::thread> threads{};
		threads.reserve(Actions.size());
		for (auto& action : Actions)
		{
			threads.emplace_back(action.GetActionInfo().Function);
		}
		for (auto& thread : threads)
		{
			thread.join();
		}
	}

	[[nodiscard]] bool TryExecute() const override
	{
		if (!Parallel)
		{
			for (auto& action : Actions)
			{
				if (!action.TryExecute()) return false;
			}
			return true;
		}

		std::vector<std::thread> threads{};
		threads.reserve(Actions.size());
		for (auto& action : Actions)
		{
			threads.emplace_back(action.GetActionInfo().Function);
		}
		for (auto& thread : threads)
		{
			thread.join();
		}
		return true;
	}

	[[nodiscard]] bool TryExecute(std::ostream& stream) const override
	{
		if (!Parallel)
		{
			for (auto& action : Actions)
			{
				if (!action.TryExecute(stream)) return false;
			}
			return true;
		}

		std::vector<std::thread> threads{};
		std::atomic_bool no_exception{true};
		threads.reserve(Actions.size());
		for (auto& action : Actions)
		{
			threads.emplace_back([&action, &no_exception, &stream]
			{
				if (!action.TryExecute(stream)) no_exception = false;
			});
		}
		for (auto& thread : threads)
		{
			thread.join();
		}
		return true;
	}
};

std::function<void()> GenerateAction(std::chrono::milliseconds time, int index)
{
	static auto counter = 0;
	static std::atomic_int progress;
	++counter;
	return [time, index]
	{
		spdlog::info("Starting action({})", index);
		std::this_thread::sleep_for(time);
		spdlog::info("Finished action({})", index);
		++progress;
		spdlog::info("Progress({}%)", static_cast<int>(static_cast<float>(progress + 1) / static_cast<float>(counter + 1) * 100));
	};
}

int main()
{
	using namespace std::chrono_literals;

	auto task1 = GenerateAction(10ms, 1);
	auto task2 = GenerateAction(100ms, 2);
	auto task3 = GenerateAction(1000ms, 3);
	ActionGroup group1{
		"Group1", 
		true, 
		{
			{"Task1", task1},
			{"Task2", task2},
			{"Task3", task3}
		}
	};

	auto task4 = GenerateAction(500ms, 4);
	auto task5 = GenerateAction(500ms, 5);
	auto task6 = GenerateAction(500ms, 6);
	const ActionGroup group2{
		"Group2",
		true,
		{
			{"Task4", task4},
			{"Task5", task5},
			{"Task6", task6}
		}
	};

	group1.Execute();
	group2.Execute();

	return 0;
}
