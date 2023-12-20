#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include "LangYa/SentryLib/TestData.hpp"
#include "LangYa/SentryLib/Common/UniqueBuffer.hpp"

int main()
{
	using namespace std::chrono_literals;

	boost::asio::io_context io_context{};
	boost::asio::ip::tcp::acceptor acceptor{
		io_context,
		{
			boost::asio::ip::make_address("127.0.0.1"),
			8990
		}
	};

	acceptor.listen();

	while (true)
	{
		try
		{
			auto client = acceptor.accept();

			spdlog::info("Referee> Found new client.");
			LangYa::SentryLib::RefereeData data;
			auto unique_buffer = LangYa::SentryLib::UniqueBuffer(data.GetSerializationResultSize());
			auto buffer_view = unique_buffer.GetView();

			data.OutpostHealth = 1000;
			data.CurrentMatchStatus = LangYa::SentryLib::RefereeData::MatchStatus::BeforeMatch;
			spdlog::info("Referee> Waiting for match.");
			while (!data.Serialize(buffer_view))
			{
				spdlog::warn("RefereeData> Serialization failed.");
			}
			for (int i = 0; i < 30; i ++)
			{
				write(client, boost::asio::buffer(buffer_view.Head, buffer_view.Size));
				std::this_thread::sleep_for(100ms);
			}

			data.OutpostHealth = 1000;
			data.CurrentMatchStatus = LangYa::SentryLib::RefereeData::MatchStatus::InMatch;
			spdlog::info("Referee> Begin match.");
			while (!data.Serialize(buffer_view))
			{
				spdlog::warn("RefereeData> Serialization failed.");
			}
			for (int i = 0; i < 30; i ++)
			{
				write(client, boost::asio::buffer(buffer_view.Head, buffer_view.Size));
				std::this_thread::sleep_for(100ms);
			}

			data.OutpostHealth = 0;
			data.CurrentMatchStatus = LangYa::SentryLib::RefereeData::MatchStatus::InMatch;
			spdlog::info("Referee> Outpost destroyed.");
			while (!data.Serialize(buffer_view))
			{
				spdlog::warn("RefereeData> Serialization failed.");
			}
			for (int i = 0; i < 30; i ++)
			{
				write(client, boost::asio::buffer(buffer_view.Head, buffer_view.Size));
				std::this_thread::sleep_for(100ms);
			}

			data.OutpostHealth = 0;
			data.CurrentMatchStatus = LangYa::SentryLib::RefereeData::MatchStatus::AfterMatch;
			spdlog::info("Referee> Match end.");
			while (!data.Serialize(buffer_view))
			{
				spdlog::warn("RefereeData> Serialization failed.");
			}
			for (int i = 0; i < 30; i ++)
			{
				write(client, boost::asio::buffer(buffer_view.Head, buffer_view.Size));
				std::this_thread::sleep_for(100ms);
			}

			client.close();
		}
		catch (...)
		{
			spdlog::error("Exception in client connection, restart new one.");
		}
	}

}
