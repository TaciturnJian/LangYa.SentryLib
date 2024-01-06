#include <atomic>

#include <spdlog/spdlog.h>

#include <LangYa/Common/LifeTimeDebugData.hpp>

std::atomic<std::uint64_t> GlobalID{0};

namespace LangYa::Common
{
	DebugData
	::DebugData()
	{
		ID = GlobalID++;
		spdlog::info("[{}:{}]> DebugData::DebugData()", Name, ID);
	}

	DebugData
	::DebugData(std::string name): Name(std::move(name))
	{
		ID = GlobalID++;
		spdlog::info("[{}:{}]> DebugData::DebugData(std::string)", Name, ID);
	}

	DebugData
	::~DebugData()
	{
		spdlog::info("[{}:{}]> DebugData::~DebugData()", Name, ID);
	}
}
