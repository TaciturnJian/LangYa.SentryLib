#include <boost/asio/ip/udp.hpp>
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
			std::thread task_send_server_data{
				[this, &client, &interrupt_signal]
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
				}
			};
			std::thread task_receive_client_data{
				[this, &client, &interrupt_signal]
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

	class CommandInterface final
	{
		IPv4Endpoint LocalEndpoint{"127.0.0.1:8990"};
		IPv4Endpoint RemoteEndpoint{"127.0.0.1:8989"};
		boost::asio::ip::udp::endpoint Target{LocalEndpoint};

		boost::asio::io_context IOContext{};
		boost::asio::ip::udp::socket Socket{
			IOContext,
			Target
		};

		UniqueBuffer Buffer{256};
		MemoryView BufferView{Buffer.GetView()};

	public:
		Action<std::istream&> OnCommandReceived{};

		void SendCommand(const std::string_view command)
		{
			BufferView.SetValue(0);
			MemoryView{command}.CopyTo(BufferView);
			Socket.send_to(boost::asio::mutable_buffer{BufferView}, Target);
		}

		void Receive()
		{
			Socket.receive_from(boost::asio::mutable_buffer{BufferView}, Target);
			std::istringstream stream{std::string{std::string_view{BufferView}}};
			OnCommandReceived(stream);
		}

		void DetachLoopReceive(std::shared_ptr<std::atomic_bool> interruptSignalPointer)
		{
			auto& interrupt_signal = *interruptSignalPointer;
			std::thread{
				[this, &interrupt_signal]
				{
					while (!interrupt_signal)
					{
						try
						{
							Receive();
						}
						catch (std::exception& ex)
						{
							spdlog::error("CommandInterface> Failed to receive command: {}", ex.what());
						}
						catch (...)
						{
							spdlog::error("CommandInterface> Failed to receive command: unknown exception");
						}
					}
				}
			}.detach();
		}
	};
}

int main()
{
	const auto interrupt_signal_pointer = std::make_shared<std::atomic_bool>(false);
	auto& interrupt_signal = *interrupt_signal_pointer;

	RefereeSystem::CommandInterface commander{};
	commander.OnCommandReceived += [&commander, &interrupt_signal](std::istream& stream)
	{
		while (!stream.eof() && !interrupt_signal)
		{
			std::string command{};
			stream >> command;
			spdlog::info("CommandInterface> Received command: {}", command);
		}
		commander.SendCommand("hello world!");
		//TODO 处理指令
	};
	commander.DetachLoopReceive(interrupt_signal_pointer);

	const auto client_handler = std::make_shared<RefereeSystem::ClientHandler>();
	client_handler->OnClientDataReceived += [](auto data)
	{
		spdlog::info("ClientHandler> Received client data!");
		//TODO 处理数据
	};

	Server{
		ServerInfo<IPv4TCPSocket>{
			std::make_shared<IPv4TCPAcceptor>(
				IPv4Endpoint{
					"127.0.0.1:8989"
				}
			),
			client_handler,
			interrupt_signal_pointer
		}
	}.Run();

	return 0;
}
