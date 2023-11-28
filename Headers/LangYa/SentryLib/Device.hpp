#pragma once

namespace LangYa::SentryLib
{
	/// @brief Represent a device that requires timer to operate. Call Device::Tick so the device will do its job.
	class Device
	{
	public:
		/// @brief Release resources in this device.
		virtual ~Device();

		/// @brief Tick the device to operate. This may block the thread that calls this function.
		///	@return Whether the device is operating.
		///	If any exception stopped this device, the tick function will return false.
		virtual bool Tick() = 0;
	};
}
