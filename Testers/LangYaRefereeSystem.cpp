#include <LangYa/Common.hpp>
#include <LangYa/Network.hpp>

#include <LangYa/RefereeSystem.h>

using namespace LangYa::Common;
using namespace LangYa::Network;

namespace RefereeSystem
{
#pragma pack(push, 1)
	struct RefereeServerData
	{
		LANGYA_PACKAGE_HEAD_TYPE Head{LANGYA_PACKAGE_HEAD};
		LANGYA_RefereeServerData Data{};
	};

	struct RefereeClientData
	{
		LANGYA_PACKAGE_HEAD_TYPE Head{LANGYA_PACKAGE_HEAD};
		LANGYA_RefereeClientData Data{};
	};
#pragma pack(pop)

	class ClientHandler final : public IClientHandler<IPv4TCPSocket>
	{
	public:
		std::shared_ptr<std::atomic<RefereeServerData>> SharedAtomicServerData{nullptr};

		Action<LANGYA_RefereeClientData> OnClientDataReceived{};

		void HandleClient(const HandleClientArgumentType& argument) override
		{
			auto& client = *argument.ClientPointer;
			const auto& interrupt_signal = *argument.InterruptSignalPointer;
			std::thread task_send_server_data{[this, &client, &interrupt_signal]
			{
				while (!interrupt_signal)
				{
					const auto& atomic_server_data = *SharedAtomicServerData;
					RefereeServerData data = atomic_server_data;
					MemoryView view{&data};
					client.Write(view);

					// 10Hz
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			}};
			std::thread task_receive_client_data{[this, &client, &interrupt_signal]
			{
				RefereeClientData data{};
				const MemoryView data_view{&data};
				const PingPongBuffer ping_pong{UniqueBuffer{sizeof(RefereeClientData)}};
				while (!interrupt_signal)
				{
					client.Read(ping_pong.GetExchangeBuffer());
					if (ping_pong.ExchangeWithExchangeBuffer(data_view))
					{
						OnClientDataReceived(data.Data);
					}
				}
			}
			};

			task_receive_client_data.join();
			task_send_server_data.join();
		}
	};
}

int main()
{
	const auto client_handler = std::make_shared<RefereeSystem::ClientHandler>();
	client_handler->OnClientDataReceived += [](auto data)
	{
		spdlog::info("ClientHandler> Received client data!");
		//TODO 处理数据
	};

	IPv4Endpoint endpoint{"127.0.0.1:8989"};
	const auto client_acceptor = std::make_shared<IPv4TCPAcceptor>(endpoint);
	const auto interrupt_signal = std::make_shared<std::atomic_bool>(false);
	const ServerInfo<IPv4TCPSocket> server_info{client_acceptor, client_handler, interrupt_signal};
	Server server{server_info};
	server.Run();
	return 0;
}
