#include <string>
#include <sstream>
#include <spdlog/spdlog.h>
#include <LangYa/SentryLib/PingPongBuffer.hpp>

#pragma pack(push, 1)
struct Data
{
	char Head{'!'};
	char A{1};
	short B{2};
	int C{3};
	float D{4};

	std::string ToString()
	{
		return fmt::format("(Head:{} A:{} B:{} C:{} D:{})", Head, static_cast<int>(A), B, C, D);
	}
};
#pragma pack(pop)

void PrintPingPong(const LangYa::SentryLib::PingPongBuffer& buffer)
{
	std::stringstream stream{};

	auto& ping = buffer.GetPing();
	auto& pong = buffer.GetPong();
	for (auto i = 0; i < ping.Size; i++)
	{
		stream << static_cast<int>(ping[i]) << ' ';
	}
	for (auto i = 0; i < ping.Size; i++)
	{
		stream << static_cast<int>(pong[i]) << ' ';
	}

	spdlog::info(stream.str());
}

int main()
{
	spdlog::info("<PingPongTester>");

	using namespace LangYa::SentryLib;

	const PingPongBuffer ping_pong{UniqueBuffer(24)};
	Data resource, destination;
	resource.A = 2;
	resource.B = 5;
	resource.C = 6;

	const MemoryView resource_view{&resource, 1};
	const MemoryView destination_view{&destination, 1};

	spdlog::info("Before exchange, destination: ");
	spdlog::info(destination.ToString());
	spdlog::info("ping pong: ");
	PrintPingPong(ping_pong);

	if (!ping_pong.Exchange(resource_view, destination_view))
	{
		spdlog::warn("Exchange failed!");
	}

	spdlog::info("After exchange, destination: ");
	spdlog::info(destination.ToString());
	spdlog::info("ping pong: ");
	PrintPingPong(ping_pong);

	spdlog::info("Modify resource head");
	resource.Head = '?';

	spdlog::info("Before exchange, resource: ");
	spdlog::info(resource.ToString());

	if (!ping_pong.Exchange(resource_view, destination_view))
	{
		spdlog::warn("Exchange failed!");
	}

	spdlog::info("ping pong: ");
	PrintPingPong(ping_pong);

	spdlog::info("Recover resource head");
	resource.Head = '!';
	spdlog::info("Before exchange, resource: ");
	spdlog::info(resource.ToString());
	spdlog::info("destination: ");
	destination = Data{};
	spdlog::info(destination.ToString());
	auto& exchange_buffer = ping_pong.GetExchangeBuffer();
	exchange_buffer.ReadFrom(resource_view);
	if (!ping_pong.ExchangeWithExchangeBuffer(destination_view))
	{
		spdlog::warn("Exchange failed!");
	}

	spdlog::info("ping pong: ");
	PrintPingPong(ping_pong);

	spdlog::info("After exchange, destination");
	spdlog::info(destination.ToString());
	spdlog::info("</PingPongTester>");
}
