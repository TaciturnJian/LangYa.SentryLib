#pragma once

namespace LangYa::SentryLib 
{
	class Device
	{
	public:
		/// @brief 释放此设备占用的空间（注意需要手动调用释放函数）。
		///	@code LangYa::SentryLib::Device::Close() @endcode
		///	否则可能导致严重错误。
		virtual ~Device() = default;

		/// @brief 在设备的系统资源已经分配完毕后开启设备。
		///	如果没有获取需要的资源，则往往设备会开启失败。
		///	@return 设备是否成功开启。
		virtual bool Open() = 0;

		/// @brief 销毁此设备占用的系统资源。
		///	此函数需要确保设备关闭或者在无法关闭时报错，往往会阻塞当前线程。
		virtual void Close() = 0;

		/// @brief 激活设备使其运作一次
		///	如果设备未开启，则此函数应当使设备开启，如果成功则开始运作。
		///	如果运作发生严重错误则抛出异常。
		///	如果设备运作一次的时间较长，则此函数会阻塞当前线程。
		///	如果你希望此设备保持运行，那么使用一个循环是最佳选择，简单的代码如下：
		///	@code
		///	//#include <thread> //引用此头文件，让当前线程可以休眠
		///	while (true) {
		///		using namespace std::chrono_literals;
		///		std::this_thread::sleep_for(1ms); //设置最大频率为 1000Hz
		///		device.Tick();
		///	}
		///	@endcode
		///	或者你可以自己实现一个稳频器。
		virtual void Tick() = 0;
	};

	class Controller : public Device
	{
	public:
		
	}; 
}