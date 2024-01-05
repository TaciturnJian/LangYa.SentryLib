#include <iostream>
#include <LangYa/SentryLib/Network.hpp>

namespace Temp
{
	class TempHandleClient final : public LangYa::SentryLib::IClientHandler<LangYa::SentryLib::IPv4TCPSocket>
	{
	public:
		void HandleClient(const HandleClientArgumentType& argument) override
		{
			auto& interrupt_signal = *argument.InterruptSignalPointer;
			auto& client = *argument.ClientPointer;

			char buffer[2];
			const LangYa::SentryLib::MemoryView view{buffer, 1};
			int counter{0};
			while (!interrupt_signal)
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
					spdlog::warn("TempClientHandler> Failed to read bytes after 10 attempts.");
					return;
				}
			}
		}
	};
}

int main(int argc, const char* argv[])
{
	using namespace LangYa::SentryLib;
	Server<IPv4TCPSocket> server{ {
		std::make_shared<IPv4TCPAcceptor>(IPv4Endpoint{argv[1]}),
		std::make_shared<Temp::TempHandleClient>(),
		std::make_shared<std::atomic_bool>(false),
		0}
	};
	server.Run();

	return 0;
}
