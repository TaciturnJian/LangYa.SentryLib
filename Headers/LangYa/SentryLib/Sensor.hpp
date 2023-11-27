#pragma once

#include <memory>

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/UniqueBuffer.hpp>
#include <LangYa/SentryLib/Serializer.hpp>
#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/Device.hpp>

namespace LangYa::SentryLib
{
	template <typename TSensorData, MemoryView::SizeType CompressedResourceSize>
	// ReSharper disable once CppClassCanBeFinal
	class Sensor final : public Device
	{
	public:
		using ConnectionType = Connection;
		using Serializer = Serializer<TSensorData, CompressedResourceSize>;

	protected:
		std::weak_ptr<ConnectionType> Connection;
		TSensorData LastData{};
		UniqueBuffer SerializerBuffer{CompressedResourceSize};

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
