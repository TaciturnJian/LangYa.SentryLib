#pragma once

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/TripleBuffer.hpp>
#include <LangYa/SentryLib/PingPongBuffer.hpp>

namespace LangYa::SentryLib
{
	class LangYaConnection final : public Connection
	{
		MemoryView::SizeType RawReceiveSize;
		MemoryView::SizeType RawSendSize;
		std::shared_ptr<Connection> SharedConnection;
		TripleBuffer ReceiveTriple;
		TripleBuffer SendTriple;

	public:
		volatile bool IsWorking{false};

		LangYaConnection(const MemoryView::SizeType rawReceiveSize, const MemoryView::SizeType rawSendSize,
		                 const std::weak_ptr<Connection>& weakConnection) :
			RawReceiveSize(rawReceiveSize),
			RawSendSize(rawSendSize),
			SharedConnection(weakConnection.lock()),
			ReceiveTriple{UniqueBuffer(rawReceiveSize * 3)},
			SendTriple{UniqueBuffer(rawSendSize * 3)}

		{
			if (SharedConnection == nullptr)
			{
				spdlog::warn("LangYaConnection> Constructed with expired connection, this connection will not work!");
			}
			else
			{
				spdlog::info("LangYaConnection> Constructed with connection({}).", SharedConnection->ToString());
			}
		}

		void HandleSending()
		{
			using namespace std::chrono_literals;

			spdlog::info("LangYaConnection> Handling sending.");

			const auto expected_sending_size = RawSendSize + 1;

			/* Buffer Structure: [ '!', <Resource> ]
			 *	The first byte is '!' to indicate the head of the sending data.
			 *	The rest bytes are for the resource.
			 */

			const UniqueBuffer data_buffer{expected_sending_size}; // Get buffer for the resource to send.
			const auto& data_view = data_buffer.GetView(); // The view of buffer.
			const MemoryView resource_view{data_view, 1, RawSendSize}; // The view of the resource in the buffer.

			data_view[0] = '!'; // Set first byte to '!' to indicate the head of the sending data.

			while (IsWorking)
			{
				std::this_thread::sleep_for(2ms); // Max sending frequency is 1000Hz.
				if (!ReceiveTriple.Pop(resource_view)) continue; // Get resource from triple buffer.

				const auto sent_bytes = SharedConnection->Write(data_view); // Send data.
				if (sent_bytes == expected_sending_size) // every bytes are sent.
					continue;

				/* ^^^ sent data normally ^^^ / vvv exception in sending vvv */

				spdlog::warn("LangYaConnection> Sent {} bytes, expected {}, some error in connection!",
				             sent_bytes,
				             expected_sending_size);

				SharedConnection->Connect();

				spdlog::warn(SharedConnection->IsConnected()
					             ? "LangYaConnection> Cannot revive connection! But continuing handling sending."
					             : "LangYaConnection> Connection revived, this sending procedure will be ignored"
				);
			}
		}

		void HandleReceiving()
		{
			using namespace std::chrono_literals;

			const auto expected_receiving_size = RawReceiveSize + 1;

			const PingPongBuffer ping_pong{UniqueBuffer{(RawReceiveSize + 1) * 2}};
			const auto& exchange_view = ping_pong.GetExchangeBuffer();
			const UniqueBuffer exchange_result{RawReceiveSize + 1};
			const auto& exchange_result_view = exchange_result.GetView();

			spdlog::info("LangYaConnection> Handling receiving.");
			while (IsWorking)
			{
				std::this_thread::sleep_for(1ms); // Max receiving frequency is 1000Hz.

				if (const auto read_bytes = SharedConnection->Read(exchange_view); read_bytes !=
					expected_receiving_size)
				{
					spdlog::warn("LangYaConnection> Receive {} bytes, expected {}, some error in connection!",
					             read_bytes, expected_receiving_size);
					SharedConnection->Connect();
					spdlog::warn(SharedConnection->IsConnected()
						             ? "LangYaConnection> Connection revived, this receiving procedure will be ignored"
						             : "LangYaConnection> Cannot revive connection! But continuing handling sending."
					);
					continue;
				}

				if (!ping_pong.ExchangeWithExchangeBuffer(exchange_result_view))
				{
					spdlog::warn("LangYaConnection> Error in received bytes, no heading found!");
					continue;
				}

				ReceiveTriple.Push(exchange_result_view);
			}
		}

		void HandleIO()
		{
			spdlog::info("LangYaConnection> Start handling communication. Creating threads.");
			std::thread sending_thread{
				[this]
				{
					spdlog::info("LangYaConnection> Sending thread joined.");
					HandleSending();
				}
			};

			std::thread receiving_thread{
				[this]
				{
					spdlog::info("LangYaConnection> Receiving thread joined.");
					HandleReceiving();
				}
			};

			spdlog::info("LangYaCommunicator> Join threads.");
			sending_thread.join();
			receiving_thread.join();
		}

		MemoryView::SizeType Read(const MemoryView& view) override
		{
			if (view.Size < RawReceiveSize)
			{
				return 0;
			}

			if (!ReceiveTriple.Pop(view))
			{
				return 0;
			}

			return RawReceiveSize;
		}

		MemoryView::SizeType Write(const MemoryView& view) override
		{
			if (view.Size < RawSendSize)
			{
				return 0;
			}

			SendTriple.Push(view);
			return view.Size;
		}

		void Connect() override
		{
			SharedConnection->Connect();
		}

		[[nodiscard]] bool IsConnected() override
		{
			return SharedConnection->IsConnected();
		}

		void Disconnect() override
		{
			SharedConnection->Disconnect();
		}
	};

	template <
		typename TReceiveData,
		typename TSendData,
		MemoryView::SizeType ActualSendSize = sizeof(TSendData) + 1,
		MemoryView::SizeType ActualReceiveSize = sizeof(TReceiveData) + 1
	>
	class LangYaCommunicator final
	{
		LangYaConnection InnerConnection;

	public:
		explicit LangYaCommunicator(const std::weak_ptr<Connection>& weakConnection) :
			InnerConnection(sizeof(TReceiveData), sizeof(TSendData), weakConnection)
		{
		}

		void Work()
		{
			InnerConnection.HandleIO();
		}

		void Stop()
		{
			InnerConnection.IsWorking = false;
		}

		void Send(const TSendData& data)
		{
			if (InnerConnection.Write(MemoryView{&data, 1}) != sizeof(TSendData))
			{
				spdlog::warn("LangYaCommunicator> Cannot send full data.");
			}
		}

		bool Receive(TReceiveData& data)
		{
			return InnerConnection.Read(MemoryView{&data, 1}) == sizeof(TReceiveData);
		}
	};
}
