#include <LangYa/SentryLib.hpp>

#include <cmath>
#include <thread>
#include <chrono>
#include <memory>
#include <iostream>
#include <sstream>

using namespace std;
using namespace LangYa::SentryLib;

struct SentryData final : SerializableContent, DeserializableContent
{
#pragma pack(push,1)
	struct ComplexFireByte
	{
		unsigned char FireFlag      : 2 = 0;
		unsigned char HoldFlag      : 2 = 0;
		unsigned char FireFrequency : 4 = 0;

		ComplexFireByte() = default;
	};

	struct SerializationResult final
	{
		char VelocityX{0};         // Integers in [-100, 100]
		char VelocityY{0};         // Integers in [-100, 100]
		float Yaw{0};              // 1 = 1 Degree
		short Pitch{0};            // 1 = 0.01 Degree
		ComplexFireByte Complex{}; // Complex byte for fire
		unsigned char CRCCode{0};  // CRC8

		void operator()(const SentryData& data)
		{
			VelocityX = data.Velocity[0];
			VelocityY = data.Velocity[1];
			Yaw = data.GimbalEulerAngle[0];
			Pitch = data.GimbalEulerAngle[1] * 100;
			Complex.FireFlag = data.FireFlag;
			Complex.FireFrequency = 0;
			Complex.HoldFlag = 0;
		}
	};

	struct DataToDeserialize final
	{
		char Head{'!'};
		float Yaw{0};                // 1 = 1°
		short Pitch{0};              // 1 = 0.01°
		unsigned short AmmoCount{0}; // 1 = 一发子弹
		char VelocityX{0};           // [-100, 100]的整数
		char VelocityY{0};           // [-100, 100]的整数
		bool FireFlag{false};        // 当前开火标志（取反后开火）
		unsigned char CRCCode{0};    // CRC8验证位置

		void operator()(SentryData& data) const
		{
			data.GimbalEulerAngle = {Yaw, static_cast<float>(Pitch) / 100.0f};
			data.Velocity = {VelocityX, VelocityY};
			data.AmmoCount = AmmoCount;
			data.FireFlag = FireFlag;
		}
	};

#pragma pack(pop)

	Vector2F GimbalEulerAngle{0, 0}; // 单位 (度, 度)
	Vector2I Velocity{0, 0};         // 单位 (档, 档)
	unsigned short AmmoCount{0};     // 单位 发
	bool FireFlag{false};            // 开火标志位

	SentryData() = default;
	SentryData(const SentryData& other) = default;
	SentryData(SentryData&& other) = default;
	SentryData& operator=(const SentryData& other) = default;
	SentryData& operator=(SentryData&& other) = default;

	[[nodiscard]] MemoryView GetSerializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}

	[[nodiscard]] MemoryView::SizeType GetSerializationResultSize() const override
	{
		return sizeof(SerializationResult);
	}

	[[nodiscard]] bool Serialize(const MemoryView& buffer) override
	{
		if (buffer.Size < sizeof(SerializationResult))
		{
			return false;
		}

		// ReSharper disable once CppUseStructuredBinding
		auto& data = *(reinterpret_cast<SerializationResult*>(buffer.Head));
		data(*this);
		return true;
	}

	[[nodiscard]] MemoryView GetDeserializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}

	[[nodiscard]] MemoryView::SizeType GetDeserializationResourceSize() const override
	{
		return sizeof(DataToDeserialize);
	}

	[[nodiscard]] bool Deserialize(const MemoryView& buffer) override
	{
		if (buffer.Size < sizeof(DataToDeserialize))
		{
			return false;
		}

		// ReSharper disable once CppUseStructuredBinding
		const auto& data = *(reinterpret_cast<DataToDeserialize*>(buffer.Head));
		data(*this);
		return true;
	}
};

int main()
{
	boost::asio::io_context serial_port_io_context{};
	auto serial_port = SerialPort::BuildShared
	(
		serial_port_io_context,
		{
			"/dev/usb_autoaim",
			115200
		}
	);
	serial_port->Connect();

	/*boost::asio::io_context tcp_io_context{};
	auto tcp = TCPConnection::BuildShared(
		tcp_io_context,
		"",
		8989
	);*/

	auto decorated_serial_port = std::make_shared<LangYaConnection>(
		sizeof(SentryData::DataToDeserialize), 
		sizeof(SentryData::SerializationResult),
		serial_port
	);

	std::vector<std::shared_ptr<Device>> devices;
	auto sensor_ptr = std::make_shared<Sensor<SentryData>>(decorated_serial_port);
	auto controller_ptr = std::make_shared<Controller<SentryData>>(decorated_serial_port);

	devices.push_back(sensor_ptr);
	devices.push_back(controller_ptr);

	std::thread connection_thread(
	[decorated_serial_port]{
		decorated_serial_port->HandleIO();
	});

	std::thread timer_thread([&devices]
	{
		using namespace std::chrono_literals;

		while (true) {
			std::this_thread::sleep_for(1ms);
			for (const auto& device_ptr: devices)
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

				float yaw = sensor->GimbalEulerAngle[0];
				float pitch = sensor->GimbalEulerAngle[1];

				//controller->GimbalEulerAngle[0] = yaw + 360 * sin(t);
				controller->GimbalEulerAngle[1] = 20 * sin(t);

				controller->Velocity[0] = 10;
				controller->Velocity[1] = 20;

				t += 0.0001;

				spdlog::info("Receive: Yaw: {}, Pitch: {}", yaw, pitch);
				spdlog::info("Sent   : Yaw: {}, Pitch: {}", controller->GimbalEulerAngle[0], controller->GimbalEulerAngle[1]);
			}
		}
	);

	connection_thread.join();
	timer_thread.join();
	operate_thread.join();

	return 0;
}
