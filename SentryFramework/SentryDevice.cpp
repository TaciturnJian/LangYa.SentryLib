#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/TestData.hpp>
#include <LangYa/SentryLib/Common/UniqueBuffer.hpp>

int main()
{
	using namespace std::chrono_literals;

	boost::asio::io_context io_context{};
	boost::asio::ip::tcp::acceptor acceptor{
		io_context,
		{
			boost::asio::ip::make_address("127.0.0.1"),
			8991
		}
	};

	acceptor.listen();

	while (true)
	{
		try
		{
			auto client = acceptor.accept();

			spdlog::info("Device> Found new client.");
			LangYa::SentryLib::FullSensorData data;
			auto unique_buffer = LangYa::SentryLib::UniqueBuffer(data.GetSerializationResultSize());
			auto buffer_view = unique_buffer.GetView();

			for (int i = 0; i < 1000; i++)
			{
				if (i % 2 == 0)
				{
					data.LatestArmorPlate.ID = LangYa::SentryLib::UnitID::Empty;
					spdlog::info("Device> No target.");
					while (!data.Serialize(buffer_view))
					{
						spdlog::warn("DeviceData> Serialization failed.");
					}
					for (int j = 0; j < 20; j++)
					{
						write(client, boost::asio::buffer(buffer_view.Head, buffer_view.Size));
						std::this_thread::sleep_for(100ms);
					}
				}
				else
				{
					data.LatestArmorPlate.ID = LangYa::SentryLib::UnitID::Hero;
					spdlog::info("Device> Hero target.");
					while (!data.Serialize(buffer_view))
					{
						spdlog::warn("DeviceData> Serialization failed.");
					}
					for (int j = 0; j < 20; j++)
					{
						write(client, boost::asio::buffer(buffer_view.Head, buffer_view.Size));
						std::this_thread::sleep_for(100ms);
					}
				}
			}

			client.close();
		}
		catch (...)
		{
			spdlog::error("Exception in client connection, restart new one.");
		}
	}
}
