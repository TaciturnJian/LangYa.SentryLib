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
						RefereeServerData data = *SharedAtomicServerData;
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
			BufferView.SetValue(' ');
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

	const auto client_handler_pointer = std::make_shared<RefereeSystem::ClientHandler>();
	auto& client_handler = *client_handler_pointer;

	RefereeSystem::CommandInterface commander{};
	commander.OnCommandReceived += [&interrupt_signal, &client_handler](std::istream& stream)
	{
		while (!stream.eof() && !interrupt_signal)
		{
			std::string ore_storage_status{};
			stream >> ore_storage_status;
			unsigned char ore_storage = 0;
			if (ore_storage_status == "open")
			{
				ore_storage = static_cast<unsigned char>(LangYa::LangYaTeamOreStorageStatusID::Open);
			}
			
			std::string team{};
			stream >> team;

			RefereeSystem::RefereeServerData data = *client_handler.SharedAtomicServerData;
			(team == "red" ? data.Data.RedTeamOreStorageStatusID : data.Data.BlueTeamOreStorageStatusID) = ore_storage;
			*client_handler.SharedAtomicServerData = data;
		}
	};
	commander.DetachLoopReceive(interrupt_signal_pointer);

	client_handler_pointer->OnClientDataReceived += [&commander](const LANGYA_RefereeClientData data)
	{
		std::stringstream stream{};
		stream << data.TeamID << ' ' << data.LocationID << ' ' << data.TaskID << ' ' << data.OreRewardID;
		commander.SendCommand(stream.str());
	};

	Server{
		ServerInfo<IPv4TCPSocket>{
			std::make_shared<IPv4TCPAcceptor>(
				IPv4Endpoint{
					"127.0.0.1:8989"
				}
			),
			client_handler_pointer,
			interrupt_signal_pointer
		}
	}.Run();

	return 0;
}
