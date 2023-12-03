#include <thread>
#include <chrono>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/LangYaConnection.hpp>

namespace LangYa::SentryLib
{
	LangYaConnection
	::LangYaConnection(
		const MemoryView::SizeType rawReceiveSize,
		const MemoryView::SizeType rawSendSize,
		const std::weak_ptr<Connection>& weakConnection
	) :
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

	void
	LangYaConnection
	::HandleSending()
	{
		using namespace std::chrono_literals;

		spdlog::info("LangYaConnection> Handling sending.");

		// The expected size for data to send. It's ['!', <Content>], with '!' to indicate the first byte of the package.
		const auto expected_sending_size = RawSendSize + 1;

		// The buffer for data to send.
		const UniqueBuffer data_buffer{expected_sending_size}; // Get buffer for the resource to send.

		// The view for data to send. 
		const auto& data_view = data_buffer.GetView(); // The view of buffer.

		// The view for the resource in the data.
		const MemoryView resource_view{data_view, 1, RawSendSize}; // The view of the resource in the buffer.

		// The minimum interval for this thread to sleep. Reduce CPU usages.
		// Determine the max frequency of sending data. The max frequency is (1 / <time>).
		constexpr auto min_interval = 2ms;

		data_view[0] = '!'; // Set first byte to '!' to indicate the head of the sending data.

		while (IsWorking)
		{
			// Sleep for the interval to reduce CPU usage.
			std::this_thread::sleep_for(min_interval);

			// Get resource from triple buffer.
			if (!SendTriple.Pop(resource_view)) continue;

			// Send the resource.
			const auto sent_bytes = SharedConnection->Write(data_view); // Send data.
			if (sent_bytes == expected_sending_size)                    // every bytes are sent.
				continue;

			/* ^^^ sent data normally ^^^ / vvv handle exception in sending vvv */

			spdlog::warn(
				"LangYaConnection> Sent {} bytes, expected {}, some error in connection!",
				sent_bytes,
				expected_sending_size);

			// Try revive the connection.
			SharedConnection->Connect();

			spdlog::warn(
				SharedConnection->IsConnected()
					? "LangYaConnection> Cannot revive connection! But continuing handling sending."
					: "LangYaConnection> Connection revived, this sending procedure will be ignored"
			);
		}
	}

	void
	LangYaConnection
	::HandleReceiving()
	{
		using namespace std::chrono_literals;

		// The expected size for received data. It's ['!', <Content>], with '!' to indicate the first byte of the package.
		const auto expected_receiving_size = RawReceiveSize + 1;

		// The ping pong buffer to find '!' easily.
		const PingPongBuffer ping_pong{UniqueBuffer{(RawReceiveSize + 1) * 2}};

		// The view for the exchange buffer in the ping pong, ready for the exchange behavior.
		const auto& exchange_view = ping_pong.GetExchangeBuffer();

		// The result buffer for the exchange behavior.
		const UniqueBuffer exchange_result{RawReceiveSize + 1};

		// The view for the exchange result buffer.
		const auto& exchange_result_view = exchange_result.GetView();

		// The view for the resource in the exchange result buffer.
		const MemoryView resource_result_view{exchange_result_view, 1, RawReceiveSize};

		// The minimum interval for this thread to sleep. Reduce CPU usages.
		// Determine the max frequency of receiving data. The max frequency is (1 / <time>).
		constexpr auto min_interval = 1ms;

		spdlog::info("LangYaConnection> Handling receiving.");
		while (IsWorking)
		{
			// Sleep for the interval to reduce CPU usage.
			std::this_thread::sleep_for(min_interval);

			// Read bytes from connection, check if the count is ok.
			if (
				const auto read_bytes = SharedConnection->Read(exchange_view);
				read_bytes != expected_receiving_size
			)
			{
				// Handle wrong byte count in connection reading.
				spdlog::warn(
					"LangYaConnection> Received {} bytes, expected {}, some error in connection!",
					read_bytes,
					expected_receiving_size
				);

				// Try revive the connection.
				SharedConnection->Connect();
				spdlog::warn(
					SharedConnection->IsConnected()
						? "LangYaConnection> Connection revived, this receiving procedure will be ignored"
						: "LangYaConnection> Cannot revive connection! But continuing handling sending."
				);

				continue;
			} /* ^^^ WrongBytesGuard ^^^ / vvv HandleCorrectBytes vvv */

			// Push bytes to ping pong and find the head of data.
			if (!ping_pong.ExchangeWithExchangeBuffer(exchange_result_view))
			{
				spdlog::warn("LangYaConnection> Error in received bytes, no heading found!");
				continue;
			}

			// Push the resource data in the exchange result view to triple buffer.
			// Attention: the resource is in the result view.
			ReceiveTriple.Push(resource_result_view);
		}
	}

	void
	LangYaConnection
	::HandleIO()
	{
		if (IsWorking)
		{
			spdlog::warn("LangYaConnection> Already working!");
			return;
		}

		IsWorking = true;
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

		spdlog::info("LangYaConnection> Join threads.");
		sending_thread.join();
		receiving_thread.join();
		IsWorking = false;
	}

	MemoryView::SizeType
	LangYaConnection
	::Read(const MemoryView& view)
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

	MemoryView::SizeType
	LangYaConnection
	::Write(const MemoryView& view)
	{
		if (view.Size < RawSendSize)
		{
			return 0;
		}

		SendTriple.Push(view);
		return view.Size;
	}

	void
	LangYaConnection
	::Connect()
	{
		SharedConnection->Connect();
	}

	bool
	LangYaConnection
	::IsConnected()
	{
		return SharedConnection->IsConnected();
	}

	void
	LangYaConnection
	::Disconnect()
	{
		SharedConnection->Disconnect();
	}
}
