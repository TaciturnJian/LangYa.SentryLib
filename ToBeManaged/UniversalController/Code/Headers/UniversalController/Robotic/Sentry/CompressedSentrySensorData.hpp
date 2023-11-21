#pragma once

#include <SangoC/Robotic/Sentry/SentrySensorData.hpp>

namespace SangoC::Robotic::Sentry
{
#pragma pack(push, 1)
	struct CompressedSentrySensorData
	{
		char Head{'!'}; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
		float Yaw{0}; // 1 = 1 Degree
		short Pitch{0}; // 1 = 0.01 Degree
		unsigned short Ammo{0};
		char VelocityX{0}; // Integers in [-100, 100]
		char VelocityY{0}; // Integers in [-100, 100]
		bool FireFlag{false};
		unsigned char CRCCode{0}; // CRC8

		CompressedSentrySensorData() = default;
		CompressedSentrySensorData(const CompressedSentrySensorData& other) = default;
		CompressedSentrySensorData(CompressedSentrySensorData&& other) = default;
		CompressedSentrySensorData& operator=(const CompressedSentrySensorData& other) = default;
		CompressedSentrySensorData& operator=(CompressedSentrySensorData&& other) = default;

		void operator()(SentrySensorData& data) const
		{
			data.GimbalEulerAngle[0] = Yaw;
			data.GimbalEulerAngle[1] = Pitch / 100.0f;
			data.Velocity[0] = VelocityX;
			data.Velocity[1] = VelocityY;
			data.Ammo = Ammo;
			data.FireFlag = FireFlag;
		}
	};
#pragma pack(pop)
}
