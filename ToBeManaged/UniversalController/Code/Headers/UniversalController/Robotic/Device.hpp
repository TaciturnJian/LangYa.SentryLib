#pragma once

namespace SangoC::Robotic {
	class Device {
	public:
		virtual ~Device() = default;
		virtual bool Tick() = 0;
	};

}
