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
	/// @brief 代表一个机器人控制器
	/// 修改此控制器的变量并在合适的时机调用
	/// @code LangYa::SentryLib::Controller::Tick() 方法，
	/// 即可发送正确的控制数据给机器人。
	///	@tparam TControllerData 控制器内可以修改的数据类型，必须是可以序列化的，
	///	有关可序列化的概念请参考 @code LangYa::SentryLib::Serializable @endcode
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
