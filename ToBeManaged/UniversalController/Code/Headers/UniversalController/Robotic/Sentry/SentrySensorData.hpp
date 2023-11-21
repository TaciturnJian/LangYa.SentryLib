#pragma once

#include <SangoC/Math/Vectors/Vector2.hpp>

namespace SangoC::Robotic::Sentry
{
	struct SentrySensorData
	{
		Math::Vectors::Vector2<float, unsigned char> GimbalEulerAngle{};
		Math::Vectors::Vector2<float, unsigned char> Velocity{};
		unsigned short Ammo{0};
		bool FireFlag{false};

		SentrySensorData() = default;
		SentrySensorData(const SentrySensorData& other) = default;
		SentrySensorData(SentrySensorData&& other) = default;
		SentrySensorData& operator=(const SentrySensorData& other) = default;
		SentrySensorData& operator=(SentrySensorData&& other) = default;
	};
}
