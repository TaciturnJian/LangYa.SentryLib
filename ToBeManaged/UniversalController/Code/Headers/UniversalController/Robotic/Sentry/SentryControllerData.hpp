#pragma once

#include <SangoC/Math/Vectors/Vector2.hpp>

namespace SangoC::Robotic::Sentry {
	struct SentryControlData
	{
		Math::Vectors::Vector2<signed char, unsigned char> Velocity{0, 0};
		Math::Vectors::Vector2<float, unsigned char> GimbalEulerAngle{0, 0};
		volatile bool FireFlag{false};
		volatile bool HoldFlag{false};
		volatile unsigned char FireFrequency{0};

		SentryControlData() = default;
		SentryControlData(const SentryControlData& other) = default;
		SentryControlData(SentryControlData&& other) = default;
		SentryControlData& operator=(const SentryControlData& other) = default;
		SentryControlData& operator=(SentryControlData&& other) = default;
	};
}