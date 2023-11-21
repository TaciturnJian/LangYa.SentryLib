#include <SangoC/Robotic/Sentry/ComplexFireByte.hpp>

SangoC::Robotic::Sentry::ComplexFireByte
::ComplexFireByte()
= default;

SangoC::Robotic::Sentry::ComplexFireByte
::ComplexFireByte(
	const unsigned char fireFlag, 
	const unsigned char holdFlag,
	const unsigned char fireFrequency
) :
	FireFlag(fireFlag),
	HoldFlag(holdFlag),
	FireFrequency(fireFrequency)
{
}

SangoC::Robotic::Sentry::ComplexFireByte
::ComplexFireByte(const ComplexFireByte& other)
= default;

SangoC::Robotic::Sentry::ComplexFireByte
::ComplexFireByte(ComplexFireByte&& other)
= default;

SangoC::Robotic::Sentry::ComplexFireByte&
SangoC::Robotic::Sentry::ComplexFireByte
::operator=(const ComplexFireByte& other)
= default;

SangoC::Robotic::Sentry::ComplexFireByte& 
SangoC::Robotic::Sentry::ComplexFireByte
::operator=(ComplexFireByte&& other)
= default;
