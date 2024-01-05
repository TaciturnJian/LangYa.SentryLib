#pragma once

#include <stdexcept>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Network/ServerInfo.hpp>
#include <LangYa/SentryLib/Counter.hpp>

namespace LangYa::SentryLib
{
	template <typename TClient>
	class Server
	{
	public:
		using ServerInfoType = ServerInfo<TClient>;

	private:
		ServerInfoType Info{};

		void CheckServerInfo()
		{
			spdlog::info("Server> Checking service info");
			if (Info.AcceptorPointer == nullptr)
			{
				throw std::invalid_argument("acceptor pointer is nullptr");
			}
			if (Info.HandlerPointer == nullptr)
			{
				throw std::invalid_argument("handler pointer is nullptr");
			}
			if (Info.InterruptSignalPointer == nullptr)
			{
				throw std::invalid_argument("interrupt pointer is nullptr");
			}
		}

	public:
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Server(const ServerInfoType& info) : Info(info)
		{
		}

		void Run()
		{
			spdlog::info("Server> Starting running");
			try
			{
				CheckServerInfo();

				spdlog::info("Server> Loading acceptor");
				auto& acceptor = *Info.AcceptorPointer;
				spdlog::info("Server> Loaded acceptor");

				spdlog::info("Server> Loading handler");
				auto& handler = *Info.HandlerPointer;
				spdlog::info("Server> Loaded handler");

				spdlog::info("Server> Loading interrupt signal");
				auto& interrupt_signal = *Info.InterruptSignalPointer;
				spdlog::info("Server> Loaded handler");

				spdlog::info("Server> Loading acceptor failure counter");
				Counter acceptor_failure_counter{Info.MaxAcceptorFailureCount};

				while (!interrupt_signal)
				{
					spdlog::info("Server> Waiting for new client");

					bool error_in_acceptor{false};
					try
					{
						auto client_pointer = acceptor.AcceptClient();
						if (client_pointer == nullptr)
						{
							throw std::invalid_argument("new client pointer is nullptr");
						}
						acceptor_failure_counter.Reset();
						try
						{
							handler.HandleClient({client_pointer, Info.InterruptSignalPointer});
						}
						catch (const std::exception& ex)
						{
							spdlog::warn("Server> Exception in handling new client: {}", ex.what());
						}
						catch (...)
						{
							spdlog::warn("Server> Exception in handling new client: unknown exception");
						}
					}
					catch (const std::exception& ex)
					{
						spdlog::warn("Server> Exception in accepting new client: {}", ex.what());
						error_in_acceptor = true;
					}
					catch (...)
					{
						spdlog::warn("Server> Exception in accepting new client: unknown exception");
						error_in_acceptor = true;
					}

					if (!error_in_acceptor || !acceptor_failure_counter.Count()) continue;

					spdlog::error("Server> Acceptor failed after {} attempts, give up", Info.MaxAcceptorFailureCount);
					break;
				}
			}
			catch (const std::exception& ex)
			{
				spdlog::error("Server> Exception in running: {}", ex.what());
			}
			catch (...)
			{
				spdlog::error("Server> Exception in running: unknown exception");
			}
			spdlog::info("Server> Stop running");
		}
	};

	template <typename TClient>
	void RunServerByServiceInfo(const ServerInfo<TClient>& info)
	{
	}
}
