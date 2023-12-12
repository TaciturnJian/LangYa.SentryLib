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
	/// @code LangYa::SentryLib::SerializableContentController::Tick() @endcode 方法，
	/// 即可发送正确的控制数据给机器人。
	///	@tparam TControllerData 控制器内可以修改的数据类型，必须是可以序列化的，
	///	有关可序列化的概念请参考 @code LangYa::SentryLib::Serializable @endcode
	template <Serializable TControllerData>
	class SerializableContentController final : public Device
	{
		/// @brief 弱连接，用于控制器向外部发送数据
		std::weak_ptr<Connection> WeakConnection;

		/// @brief 最新的要发送的数据
		TControllerData LatestData{};

		/// @brief 用于序列化发送数据的缓冲区
		UniqueBuffer SerializationBuffer;

	public:
		/// @brief 给定控制器的连接，构造一个控制器
		///	@param connection 控制器发送数据时使用的连接
		explicit SerializableContentController(const std::weak_ptr<Connection>& connection);

		/// @brief 更新控制数据
		void SetData(const TControllerData& data);

		/// @brief 更新控制器，将控制器的数据发送给机器人
		///	@return 是否成功发送
		bool Tick() override;
	};

	//TODO 检测连接异常，如果异常则选择处理方式

#pragma region 函数实现

	template <Serializable TControllerData>
	SerializableContentController<TControllerData>::SerializableContentController(const std::weak_ptr<Connection>& connection):
		WeakConnection(connection),
		SerializationBuffer(LatestData.GetSerializationResultSize())
	{
	}

	template <Serializable TControllerData>
	void SerializableContentController<TControllerData>::SetData(const TControllerData& data)
	{
		LatestData = data;
	}

	template <Serializable TControllerData>
	bool SerializableContentController<TControllerData>::Tick()
	{
		// 确认连接资源正常
		const auto connection = WeakConnection.lock();
		if (connection == nullptr)
		{
			spdlog::warn("SerializableContentController> Cannot get valid connection!");
			return false;
		}

		// 确认连接正常，不正常则重试 5 次
		for (int i = 0; i <= 5 && !connection->IsConnected(); i++)
		{
			// 第六次则放弃
			if (i == 5)
			{
				spdlog::warn("SerializableContentController> Give up after 5 attempts");
				return false;
			}

			//TODO 连续两次重试的间隔

			// 重新尝试连接到目标
			spdlog::warn("SerializableContentController> Try connect to the target. {}/{}", i, 5);
			connection->Connect();
		}

		// 从缓冲区中获取视图
		const auto& view = SerializationBuffer.GetView();

		// 序列化控制数据到缓冲区
		if (TControllerData copy_data{LatestData}; !copy_data.Serialize(view))
		{
			spdlog::warn("SerializableContentController> Serialization failed!");
			return false;
		}

		// 发送缓冲区数据
		if (connection->Write(view) < view.Size)
		{
			spdlog::warn("SerializableContentController> Cannot write all bytes to connection!");
			return false;
		}

		return true;
	}

#pragma endregion

}
