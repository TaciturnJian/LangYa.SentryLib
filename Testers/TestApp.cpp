#include <iostream>
#include <iomanip>

#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <LangYa/SentryLib.hpp>

using namespace LangYa::SentryLib;
using namespace std::chrono_literals;

ApplicationInfo AppInfo{
	"TestApp",
	"An application for tests in sentry lib.",
	{1, 0, 0, 0}
};
std::string JsonFilePath{"./config.json"};

int AStep = 1;
float BStep = 0.1f;

bool MappingJsonToVariables(boost::json::value& json)
{
#define SC_SAME_NAME_MAPPING(x) spdlog::info("Mapping> {} -> {}", #x, #x); x = json.at(#x).

	SC_SAME_NAME_MAPPING(AStep) as_int64();
	SC_SAME_NAME_MAPPING(BStep) as_double();

#undef SC_SAME_NAME_MAPPING

	return true;
}

constexpr auto MaxLogFileSize = sizeof(char) * 1024 * 1024 * 5;
constexpr auto MaxLogFileCount = 10;

bool InitializeLogger()
{
	try
	{
		std::vector<spdlog::sink_ptr> sinks{};
		sinks.push_back(
			std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
				"logs/basic-log.txt", MaxLogFileSize, MaxLogFileCount
			)
		);
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		const auto logger = std::make_shared<spdlog::logger>("multi-logger", std::begin(sinks), std::end(sinks));
		set_default_logger(logger);
		return true;
	}
	catch (...)
	{
		std::cout << "Error in logging!";
		return false;
	}
}

int main(int argc, char** argv)
{
	if (!InitializeLogger()) return -1;

	// 打印程序的基本信息
	AppInfo.OutputTo_spdlog();

	// 处理应用程序配置
	// 注意，大括号中间有些部分会导致程序提前退出。
	{
		spdlog::info("{}> Parsing command line arguments", AppInfo.Name);
		Configurator configurator{};
		configurator.AddOption()
			("help", "display all help")
			(
				"ConfigPath",
				boost::program_options::value(&JsonFilePath),
				"File path for the configuration file. Maybe the absolute path will be better"
			);
		if (!configurator.Load({argc, argv}))
		{
			spdlog::error("{}> Failed to parse command line arguments", AppInfo.Name);
			return -1;
		}
		if (configurator("help"))
		{
			spdlog::info("{}> {}", AppInfo.Name, configurator.GetHelpContent());
			return 0;
		}
		if (!Configurator::Load(JsonFilePath, MappingJsonToVariables))
		{
			spdlog::error("{}> Failed to load configuration file", AppInfo.Name);
			return -1;
		}
	}

#pragma pack(push, 1)
	struct DataType : SerializableContent
	{
		int A;
		float B;

		[[nodiscard]] MemoryView::SizeType GetSerializationResultSize() const override
		{
			return sizeof(int) + sizeof(float);
		}

		[[nodiscard]] bool Serialize(const MemoryView& buffer) override
		{
			if (buffer.Size < GetSerializationResultSize()) return false;
			MemoryView{buffer, 0, sizeof(int)}.ReadFrom(&A);
			MemoryView{buffer, sizeof(int), sizeof(float)}.ReadFrom(&B);
			return true;
		}
	};
#pragma pack(pop)

	const UniqueBuffer buffer{DataType{}.GetSerializationResultSize()};
	auto buffer_view = buffer.GetView();
	auto memory_access = std::make_shared<MemoryAccess>(buffer_view);
	auto controller = std::make_shared<Controller<DataType>>(memory_access);

	std::thread device_thread{
		[controller]
		{
			while (true)
			{
				controller->Tick();
				std::this_thread::sleep_for(1ms);
			}
		}
	};
	std::thread operate_thread{
		[controller]
		{
			auto& con = *controller;
			while (true)
			{
				con->A += AStep;
				con->B += BStep;
				std::this_thread::sleep_for(100ms);
			}
		}
	};
	std::thread monitor_thread{
		[buffer_view]
		{
			const UniqueBuffer my_buffer{buffer_view.Size};
			const auto& my_buffer_view = my_buffer.GetView();
			while (true)
			{
				std::stringstream stream;
				my_buffer_view.ReadFrom(buffer_view);
				for (MemoryView::SizeType i = 0; i < buffer_view.Size; i++)
				{
					stream << std::setw(3) << static_cast<int>(my_buffer_view[i]) << ' ';
				}
				std::cout << "Monitor> " << stream.str() << "\r";
				std::this_thread::sleep_for(5ms);
			}
		}
	};

	device_thread.join();
	operate_thread.join();
	monitor_thread.join();


	return 0;
}
