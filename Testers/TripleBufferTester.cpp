#include <map>
#include <thread>
#include <iostream>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Common/TripleBuffer.hpp>

using namespace LangYa::SentryLib;
using namespace std::chrono_literals;

int main()
{
	spdlog::info("<TripleBufferTester>");

	spdlog::info("<LargeMemoryAccessTest>");
	{
		constexpr auto single_content_size = sizeof(int) * 1024;
		constexpr auto buffer_memory_size = single_content_size * 3;
		spdlog::info("creating large triple buffer");
		auto triple_buffer_ptr = std::make_shared<TripleBuffer>(UniqueBuffer{buffer_memory_size});
		auto end_signal = std::make_shared<std::atomic_bool>();

		spdlog::info("creating threads for triple buffer");
		std::thread writer{
			[triple_buffer_ptr, end_signal]
			{
				spdlog::info("writer> sleep 1s for reader");
				std::this_thread::sleep_for(1s);

				const UniqueBuffer unique_buffer{single_content_size};
				auto& view = unique_buffer.GetView();
				const auto number = reinterpret_cast<int*>(view.Head);
				auto& signal = *end_signal;
				spdlog::info("writer> start writing");
				for (int i = 0; i < 1000; i++)
				{
					std::cout << '[' << i + 1 << "/1000]\r";
					number[0] = i + 1;
					triple_buffer_ptr->Push(view);
					std::this_thread::sleep_for(1ms);
				}
				std::cout << '\n';
				spdlog::info("writer> send end signal");
				signal = true;
			}
		};
		std::thread reader{
			[triple_buffer_ptr, end_signal]
			{
				const UniqueBuffer unique_buffer{single_content_size};
				auto& view = unique_buffer.GetView();
				const auto number = reinterpret_cast<int*>(view.Head);
				const auto& signal = *end_signal;
				std::map<int, bool> numbers;
				for (int i = 1; i <= 1000; i++)
				{
					numbers[i] = false;
				}

				spdlog::info("reader> ready to receive");
				while (!signal)
				{
					if (!triple_buffer_ptr->Pop(view)) continue;

					auto receive = number[0];
					numbers[receive] = true;
					std::this_thread::sleep_for(1ms);
				}

				spdlog::info("reader> received end signal");
				std::vector<int> unreceived{};
				for (int i = 1; i <= 1000; i++)
				{
					if (!numbers[i])
					{
						unreceived.push_back(i);
						spdlog::info("reader> lost {}", i);
					}
				}

				const auto loss = static_cast<double>(unreceived.size()) / 1000.0;
				spdlog::info("reader> loss: {}%", loss * 100);
			}
		};
		spdlog::info("starting reader and writer");
		writer.join();
		reader.join();
	}
	spdlog::info("</LargeMemoryAccessTest>");

	spdlog::info("</TripleBufferTester>");
	return 0;
}
