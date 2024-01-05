#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/DynamicStack.hpp>
#include <LangYa/SentryLib/DebugData.hpp>

int main()
{
	using namespace LangYa::SentryLib;

	std::atomic_bool isTerminatedSignal{false};

	{
		DynamicStack<DebugData> stack{};

		spdlog::info("Main> Get available element 0");

		auto element_0_ptr = std::make_shared<DebugData>("Element0");
		auto& space0 = stack.RestoreSpace();
		space0 = element_0_ptr;

		auto element_1_ptr = std::make_shared<DebugData>("Element1");
		auto& space1 = stack.RestoreSpace();
		space1 = element_1_ptr;

		std::thread terminate_after_3s{
			[element_0_ptr, &isTerminatedSignal]
			{
				auto& element = *element_0_ptr;
				spdlog::info("Thread> Element0 shared pointer in this thread will be destroyed after 3s");
				std::this_thread::sleep_for(std::chrono::seconds{3});
				spdlog::info("Thread> Element0 shared pointer in this thread has been destroyed");
				isTerminatedSignal = true;
			}
		};
		terminate_after_3s.detach();
		std::this_thread::sleep_for(std::chrono::seconds{4});
		if (isTerminatedSignal)
		{
			spdlog::info("Main> Begin to destroy stack after sub thread terminated");
		}
		else
		{
			spdlog::info("Main> Begin to destroy stack before sub thread terminated");
		}
	}
	spdlog::info("Main> Complete destroying stack");

	spdlog::info("Main> Checking sub thread status");
	while (!isTerminatedSignal)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds{10});
	}
	spdlog::info("Main> Exiting");
}
