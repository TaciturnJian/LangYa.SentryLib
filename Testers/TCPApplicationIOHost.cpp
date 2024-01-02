#include <iostream>
#include <LangYa/SentryLib/Common/UniqueBuffer.hpp>
#include <LangYa/SentryLib/Network.hpp>

int main(int argc, const char* argv[])
{
	using namespace LangYa::SentryLib;
	IPv4Endpoint endpoint{};
	endpoint.Parse(argv[1]);

	IPv4TCPServer server{endpoint};

	std::atomic_bool interrupt_signal = false;
	server.Run(interrupt_signal, [&interrupt_signal](IPv4TCPSocket& client, const std::atomic_bool& interruptSignal)
	{
		char buffer[2];
		const MemoryView view{buffer, 1};
		int counter{0};
		while (!interruptSignal)
		{
			if (client.Read(view) > 0)
			{
				std::cout << buffer[0];
				if (buffer[0] == 127)
				{
					interrupt_signal = true;
				}
				counter = 0;
				continue;
			}
			counter++;
			if (counter == 5)
			{
				spdlog::warn("ClientHandler> Failed to read bytes after 10 attempts.");
				return;
			}
		}
	});

	return 0;
}
