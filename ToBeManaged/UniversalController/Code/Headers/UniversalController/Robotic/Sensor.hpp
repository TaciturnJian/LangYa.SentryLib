#pragma once

#include <memory>

#include <SangoC/Memory/MemoryView.hpp>
#include <SangoC/Memory/Buffers/UniqueBuffer.hpp>
#include <SangoC/Memory/Serializer.hpp>
#include <SangoC/Communication/Connections/Connection.hpp>
#include <SangoC/Robotic/Device.hpp>

namespace SangoC::Robotic
{
	template <typename TSensorData, Memory::MemoryView::SizeType CompressedResourceSize>
	// ReSharper disable once CppClassCanBeFinal
	class Sensor final : public Device
	{
	public:
		using ConnectionType = Communication::Connections::Connection;
		using Serializer = Memory::Serializer<TSensorData, CompressedResourceSize>;

	protected:
		std::weak_ptr<ConnectionType> Connection;
		TSensorData LastData{};
		Memory::Buffers::UniqueBuffer SerializerBuffer{CompressedResourceSize};

	public:
		explicit Sensor(const std::weak_ptr<ConnectionType>& connection) : Connection(connection)
		{
		}

		bool Tick() override
		{
			const auto connection = Connection.lock();

			if (connection == nullptr)
			{
				spdlog::warn("Sensor> Cannot get connection!");
				return false;
			}

			const auto& view = SerializerBuffer.GetView();

			if (connection->Read(view) < view.Size)
			{
				spdlog::warn("Sensor> Cannot read enough bytes from connection!");
				return false;
			}

			if (!Serializer::Deserialize(view, LastData))
			{
				spdlog::warn("Sensor> Deserialization failed!");
				return false;
			}

			return true;
		}

		TSensorData* operator->()
		{
			return &LastData;
		}
	};
}
