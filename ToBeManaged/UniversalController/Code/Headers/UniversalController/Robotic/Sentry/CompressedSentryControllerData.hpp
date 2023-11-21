#pragma once

#include <SangoC/Robotic/Sentry/SentryControllerData.hpp>
#include <SangoC/Robotic/Sentry/ComplexFireByte.hpp>

namespace SangoC::Robotic::Sentry
{
#pragma pack(push, 1)

	struct CompressedSentryControllerData
	{
		char Head{'!'}; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
		char VelocityX{0}; // Integers in [-100, 100]
		char VelocityY{0}; // Integers in [-100, 100]
		float Yaw{0}; // 1 = 1 Degree
		short Pitch{0}; // 1 = 0.01 Degree
		ComplexFireByte Complex{}; // Complex byte for fire
		unsigned char CRCCode{0}; // CRC8

		CompressedSentryControllerData();

		explicit CompressedSentryControllerData(const SentryControlData& other);

		CompressedSentryControllerData(const CompressedSentryControllerData& other);

		CompressedSentryControllerData(CompressedSentryControllerData&& other) noexcept;

		CompressedSentryControllerData& operator=(const CompressedSentryControllerData& other);

		CompressedSentryControllerData& operator=(CompressedSentryControllerData&& other) noexcept;

		CompressedSentryControllerData& operator=(const SentryControlData& other);
	};

#pragma pack(pop)
}
