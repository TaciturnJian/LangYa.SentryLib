#include <SangoC/Robotic/Sentry/CompressedSentryControllerData.hpp>

SangoC::Robotic::Sentry::CompressedSentryControllerData
::CompressedSentryControllerData() :
	Complex(0, 0, 0)
{
}

SangoC::Robotic::Sentry::CompressedSentryControllerData
::CompressedSentryControllerData(const SentryControlData& other) :
	VelocityX(other.Velocity[0]),
	VelocityY(other.Velocity[1]),
	Yaw(other.GimbalEulerAngle[0]),
	Pitch(static_cast<short>(other.GimbalEulerAngle[1] * 100.0f)),
	Complex(
		other.FireFlag ? 1 : 0,
		other.HoldFlag ? 1 : 0,
		other.FireFrequency
	)
{
}

SangoC::Robotic::Sentry::CompressedSentryControllerData
::CompressedSentryControllerData(const CompressedSentryControllerData& other) :
	VelocityX(other.VelocityX),
	VelocityY(other.VelocityY),
	Yaw(other.Yaw),
	Pitch(other.Pitch),
	Complex(other.Complex),
	CRCCode(other.CRCCode)
{
}

SangoC::Robotic::Sentry::CompressedSentryControllerData
::CompressedSentryControllerData(CompressedSentryControllerData&& other) noexcept :
	VelocityX(other.VelocityX),
	VelocityY(other.VelocityY),
	Yaw(other.Yaw),
	Pitch(other.Pitch),
	Complex(other.Complex),
	CRCCode(other.CRCCode)
{
}

SangoC::Robotic::Sentry::CompressedSentryControllerData&
SangoC::Robotic::Sentry::CompressedSentryControllerData
::operator=(const CompressedSentryControllerData& other)
{
	if (this == &other) return *this;

	VelocityX = other.VelocityX;
	VelocityY = other.VelocityY;
	Yaw = other.Yaw;
	Pitch = other.Pitch;
	Complex = other.Complex;
	CRCCode = other.CRCCode;

	return *this;
}

SangoC::Robotic::Sentry::CompressedSentryControllerData& 
SangoC::Robotic::Sentry::CompressedSentryControllerData
::operator=(CompressedSentryControllerData&& other) noexcept
{
	if (this == &other) return *this;

	VelocityX = other.VelocityX;
	VelocityY = other.VelocityY;
	Yaw = other.Yaw;
	Pitch = other.Pitch;
	Complex = other.Complex;
	CRCCode = other.CRCCode;

	return *this;
}

SangoC::Robotic::Sentry::CompressedSentryControllerData&
SangoC::Robotic::Sentry::CompressedSentryControllerData
::operator=(const SentryControlData& other)
{
	VelocityX = other.Velocity[0];
	VelocityY = other.Velocity[1];
	Yaw = other.GimbalEulerAngle[0];
	Pitch = static_cast<short>(other.GimbalEulerAngle[1] * 100.0f);
	Complex.FireFlag = other.FireFlag ? 1 : 0;
	Complex.HoldFlag = other.HoldFlag ? 1 : 0;
	Complex.FireFrequency = other.FireFrequency;
	return *this;
}
