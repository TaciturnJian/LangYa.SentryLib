#pragma once

#include <memory>

#include <LangYa/SentryLib/Common/Connection.hpp>
#include <LangYa/SentryLib/Common/DeserializableContent.hpp>
#include <LangYa/SentryLib/Device.hpp>
#include <LangYa/SentryLib/Common/MemoryView.hpp>
#include <LangYa/SentryLib/Common/UniqueBuffer.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一个机器人传感器。
	///	TODO 注释与文档
	///	Tick this device so it will update sensor data from connection, read the data from its members.
	template <Deserializable TSensorData>
	class DeserializableContentSensor final : public Device
	{
		std::weak_ptr<Connection> WeakConnection;
		TSensorData LatestData{};
		UniqueBuffer SerializationBuffer;

	public:
		explicit DeserializableContentSensor(const std::weak_ptr<Connection>& connection);

		void GetData(TSensorData& data);

		bool Tick() override;

		TSensorData* operator->()
		{
			return &LatestData;
		}
	};

	template <Deserializable TSensorData>
	DeserializableContentSensor<TSensorData>::DeserializableContentSensor(const std::weak_ptr<Connection>& connection):
		WeakConnection(connection),
		SerializationBuffer(LatestData.GetDeserializationResourceSize())
	{
	}

	template <Deserializable TSensorData>
	void DeserializableContentSensor<TSensorData>::GetData(TSensorData& data)
	{
		data = LatestData;
	}

	template <Deserializable TSensorData>
	bool DeserializableContentSensor<TSensorData>::Tick()
	{
		const auto connection = WeakConnection.lock();

		if (connection == nullptr)
		{
			spdlog::warn("DeserializableContentSensor> Cannot get connection!");
			return false;
		}

		const auto& view = SerializationBuffer.GetView();

		if (connection->Read(view) < view.Size)
		{
			spdlog::warn("DeserializableContentSensor> Cannot read enough bytes from connection!");
			return false;
		}

		if (!LatestData.Deserialize(view))
		{
			spdlog::warn("DeserializableContentSensor> Deserialization failed!");
			return false;
		}

		return true;
	}
}
