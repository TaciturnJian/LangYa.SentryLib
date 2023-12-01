#pragma once

#include <memory>

#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/DeserializableContent.hpp>
#include <LangYa/SentryLib/Device.hpp>
#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/UniqueBuffer.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一个机器人传感器。
	///	Tick this device so it will update sensor data from connection, read the data from its members.
	template <Deserializable TSensorData>
	class Sensor final : public Device
	{
		std::weak_ptr<Connection> WeakConnection;
		TSensorData LatestData{};
		UniqueBuffer SerializationBuffer;

	public:
		explicit Sensor(const std::weak_ptr<Connection>& connection) :
			WeakConnection(connection),
			SerializationBuffer(LatestData.GetDeserializationResourceSize())
		{
		}

		bool Tick() override
		{
			const auto connection = WeakConnection.lock();

			if (connection == nullptr)
			{
				spdlog::warn("Sensor> Cannot get connection!");
				return false;
			}

			const auto& view = SerializationBuffer.GetView();

			if (connection->Read(view) < view.Size)
			{
				spdlog::warn("Sensor> Cannot read enough bytes from connection!");
				return false;
			}

			if (!LatestData.Deserialize(view))
			{
				spdlog::warn("Sensor> Deserialization failed!");
				return false;
			}

			return true;
		}

		TSensorData* operator->()
		{
			return &LatestData;
		}
	};
}
