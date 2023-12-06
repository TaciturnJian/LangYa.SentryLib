#pragma once

#include <LangYa/SentryLib/CSharpp.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一种虚拟的设备，它需要有一个外部循环去调用。
	///	大多数使用情况都是在一个线程中调用 Tick() 方法。
	SC_INTERFACE Device
	{
	public:
		/// @brief 释放设备的所有资源。
		virtual ~Device();

		/// @brief 调用此方法，设备会在短时间内阻塞线程完成工作。
		///	@return 设备是否工作正常，任何异常都会导致此值为 false 。
		///	当出现多次异常时，考虑将此设备标记为不可用，或刷新此设备。
		virtual bool Tick() = 0;
	};
}
