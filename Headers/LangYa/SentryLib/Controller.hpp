#pragma once

#include <memory>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/UniqueBuffer.hpp>
#include <LangYa/SentryLib/SerializableContent.hpp>
#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/Device.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent an controller for robot.
	/// Change the control data in this class and call tick to send message. 
	template<Serializable TControllerData> 
	// ReSharper disable once CppClassCanBeFinal
	class Controller final : public Device
	{
		std::weak_ptr<Connection> WeakConnection;
		TControllerData LastData{};
		UniqueBuffer SerializationBuffer;

	public:
		explicit Controller(const std::weak_ptr<Connection>& connection) :
			WeakConnection(connection),
			SerializationBuffer(LastData.GetSerializationResultSize())
		{
		}

		bool Tick() override
		{
			const auto connection = WeakConnection.lock();

			if (connection == nullptr)
			{
				spdlog::warn("Controller> Cannot get valid connection!");
				return false;
			}

			for (int i = 0; i <= 5 && !connection->IsConnected(); i++)
			{
				if (i == 5)
				{
					spdlog::warn("Controller> Give up after 5 attempts");
					return false;
				}

				spdlog::warn("Controller> Try connect to the target. {}/{}", i, 5);
				connection->Connect();
			}

			const auto& view = SerializationBuffer.GetView();

			if (!LastData.Serialize(view))
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

		TControllerData* operator->()
		{
			return &LastData;
		}
	};
}
