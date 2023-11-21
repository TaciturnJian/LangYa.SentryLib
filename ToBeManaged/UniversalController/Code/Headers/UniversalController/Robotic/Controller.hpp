#pragma once

#include <memory>

#include <spdlog/spdlog.h>

#include <SangoC/Memory/MemoryView.hpp>
#include <SangoC/Memory/Buffers/UniqueBuffer.hpp>
#include <SangoC/Memory/Serializer.hpp>
#include <SangoC/Communication/Connections/Connection.hpp>
#include <SangoC/Robotic/Device.hpp>

namespace SangoC::Robotic
{
	template<typename TControlData, Memory::MemoryView::SizeType CompressedResourceSize> 
	// ReSharper disable once CppClassCanBeFinal
	class Controller final : public Device
	{
	public:
		using ConnectionType = Communication::Connections::Connection;
		using Serializer = Memory::Serializer<TControlData, CompressedResourceSize>;

	protected:
		std::weak_ptr<ConnectionType> Connection;
		TControlData LastData{};
		Memory::Buffers::UniqueBuffer SerializerBuffer{CompressedResourceSize};
		volatile bool Power{false};

	public:
		explicit Controller(const std::weak_ptr<ConnectionType>& connection) : Connection(connection)
		{
		}

		bool Tick() override
		{
			if (!Power) return true;

			const auto connection = Connection.lock();

			if (connection == nullptr)
			{
				spdlog::warn("Controller> Cannot get connection!");
				return false;
			}

			const auto& view = SerializerBuffer.GetView();

			if (!Serializer::Serialize(LastData, view))
			{
				spdlog::warn("Controller> Serialization failed!");
				return false;
			}

			if (connection->Write(view) < view.Size)
			{
				spdlog::warn("Controller> Cannot write all bytes to connection!");
				return false;
			}

			return true;
		}

		void Switch(const bool power)
		{
			Power = power;
		}

		TControlData* operator->()
		{
			return &LastData;
		}
	};
}
