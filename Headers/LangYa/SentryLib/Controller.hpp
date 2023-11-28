#pragma once

#include <memory>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/UniqueBuffer.hpp>
#include <LangYa/SentryLib/Serializer.hpp>
#include <LangYa/SentryLib/Connection.hpp>
#include <LangYa/SentryLib/Device.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent an controller for robot.
	/// Change the control data in this class and call tick to send message. 
	template<typename TControlData> 
	// ReSharper disable once CppClassCanBeFinal
	class Controller final : public Device
	{
	public:
		/// @brief Represent the type of serializer, which will be used in encoding and decoding data.
		using Serializer = Serializer<TControlData, CompressedResourceSize>;

	protected:
		std::weak_ptr<Connection> WeakConnection;
		TControlData LastData{};
		UniqueBuffer SerializerBuffer{CompressedResourceSize};

	public:
		explicit Controller(const std::weak_ptr<Connection>& connection) :
			WeakConnection(connection)
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

		TControlData* operator->()
		{
			return &LastData;
		}
	};
}
