#pragma once

#include <LangYa/SentryLib/LangYaConnection.hpp>

namespace LangYa::SentryLib {
	/// @brief 一个类的装饰器： @code LangYa::SentryLib::LangYaConnection @endcode 。
	///	@tparam TReceiveData The type for the content to receive.
	///	@tparam TSendData The type for the content to send.
	template <
		typename TReceiveData,
		typename TSendData
	>
	class LangYaCommunicator final
	{
		/// @brief The lang ya connection to decorate.
		LangYaConnection InnerConnection;

	public:
		/// @brief Construct a lang ya connection by the given weak connection.
		explicit LangYaCommunicator(const std::weak_ptr<Connection>& weakConnection) :
			InnerConnection(sizeof(TReceiveData), sizeof(TSendData), weakConnection)
		{
		}

		/// @brief Start working.
		void Work()
		{
			InnerConnection.HandleIO();
		}

		/// @brief Stop working.
		void Stop()
		{
			InnerConnection.IsWorking = false;
		}

		/// @brief Send the given data.
		///	@param data The data to send.
		void Send(const TSendData& data)
		{
			if (InnerConnection.Write(MemoryView{&data, 1}) != sizeof(TSendData))
			{
				spdlog::warn("LangYaCommunicator> Cannot send full data.");
			}
		}

		/// @brief Try receive the data.
		/// @param data The mutable data buffer to write the receive data.
		///	@return Whether received some data.
		bool Receive(TReceiveData& data)
		{
			return InnerConnection.Read(MemoryView{&data, 1}) == sizeof(TReceiveData);
		}
	};
}