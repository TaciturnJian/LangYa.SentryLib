#include <LangYa/SentryLib/Network.hpp>

int main(int argc, const char* argv[]) {
	using namespace LangYa::SentryLib;
	using namespace std::chrono_literals;

	IPv4TCPSocket socket{IPv4Endpoint{argv[1]}};
	if (!socket.Open())
	{
		spdlog::error("Main> Cannot open socket to test connection");
	}

	char buffer[1];
	const MemoryView view{buffer, 1};

	while (buffer[0] != 127)
	{
		buffer[0]++;
		socket.Write(view);
		std::this_thread::sleep_for(10ms);
	}
} 