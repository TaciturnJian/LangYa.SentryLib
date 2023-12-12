#include <cmath>
#include <thread>
#include <chrono>
#include <memory>

#include <LangYa/SentryLib.hpp>

using namespace std;
using namespace LangYa::SentryLib;

void TestServer()
{
	using namespace std::chrono_literals;

	boost::asio::io_context tcp_io_context{};
	boost::asio::ip::tcp::acceptor client_acceptor{tcp_io_context};
	boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address("127.0.0.1"), 8989};
	client_acceptor.open(endpoint.protocol());
	client_acceptor.bind(endpoint);
	client_acceptor.listen();

	while (true)
	{
		boost::asio::ip::tcp::socket socket{client_acceptor.accept()};
		std::thread client_thread(
			[&socket]
			{
				auto socket_ptr = std::make_shared<TCPConnection>(std::move(socket));
				const auto client = std::make_shared<LangYaConnection>(
					sizeof(SentryData::DeserializeResource),
					sizeof(SentryData::SerializationResult),
					socket_ptr
				);
				std::thread io_handle{[client] { client->HandleIO(); }};
				io_handle.detach();

				const UniqueBuffer buffer{sizeof(SentryData::DeserializeResource)};
				auto& buffer_view = buffer.GetView();
				SentryData data;

				while (true)
				{
					std::this_thread::sleep_for(1ms);
					if (client->Read(buffer_view) == 0)
					{
						spdlog::warn("Server> Cannot read data from buffer.");
						continue;
					}

					if (!data.Deserialize(buffer_view))
					{
						spdlog::warn("Server> Failed to deserialize data.");
						continue;
					}
				}
			}
		);
		client_thread.detach();
		std::this_thread::sleep_for(1s);
	}
}

int main(const int argc, const char** argv)
{
	/*boost::asio::io_context serial_port_io_context{};
	auto serial_port = SerialPort::BuildShared
	(
		serial_port_io_context,
		{
			"/dev/usb_autoaim",
			115200
		}
	);
	serial_port->Connect();*/

	ApplicationInfo{
		"ExampleApp",
		"An application with sentry lib!",
		{1, 0, 0, 0}
	}.OutputTo_spdlog();

	int a{0};
	Configurator cfg{};
	cfg.AddOption()("help", "display tips.")("a", boost::program_options::value(&a), "test");
	cfg.Load({argc, argv});


	spdlog::info("Value for test: {}", a);

	std::thread server_thread(
		TestServer
	);

	server_thread.detach();

	boost::asio::io_context tcp_io_context{};
	auto tcp = TCPConnection::BuildShared(
		tcp_io_context,
		"127.0.0.1",
		8989
	);
	tcp->Connect();

	auto decorated_serial_port = std::make_shared<LangYaConnection>(
		sizeof(SentryData::DeserializeResource),
		sizeof(SentryData::SerializationResult),
		tcp
	);

	std::vector<std::shared_ptr<Device>> devices;
	auto sensor_ptr = std::make_shared<DeserializableContentSensor<SentryData>>(decorated_serial_port);
	auto controller_ptr = std::make_shared<SerializableContentController<SentryData>>(decorated_serial_port);

	devices.push_back(sensor_ptr);
	devices.push_back(controller_ptr);

	std::thread connection_thread(
		[decorated_serial_port]
		{
			decorated_serial_port->HandleIO();
		});

	std::thread timer_thread([&devices]
	{
		using namespace std::chrono_literals;

		while (true)
		{
			std::this_thread::sleep_for(1ms);
			for (const auto& device_ptr : devices)
			{
				device_ptr->Tick();
			}
		}
	});

	std::thread operate_thread(
		[sensor_ptr, controller_ptr]
		{
			double t = 0;
			auto& sensor = *sensor_ptr;
			auto& controller = *controller_ptr;
			while (true)
			{
				std::this_thread::sleep_for(1ms);

				/*float yaw = sensor->GimbalEulerAngle[0];
				float pitch = sensor->GimbalEulerAngle[1];

				controller->GimbalEulerAngle = {};[0] = yaw + 360 * sin(t);
				controller->GimbalEulerAngle[1] = 7.5 + 12.5 * sin(t);

				controller->Velocity[0] = 10;
				controller->Velocity[1] = 20;

				t += 0.0001;

				spdlog::info("Receive: Yaw: {}, Pitch: {}", yaw, pitch);
				spdlog::info("Sent   : Yaw: {}, Pitch: {}", controller->GimbalEulerAngle[0],
				             controller->GimbalEulerAngle[1]);*/
			}
		}
	);

	connection_thread.join();
	timer_thread.join();
	operate_thread.join();

	return 0;
}
