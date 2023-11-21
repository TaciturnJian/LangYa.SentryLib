#pragma once

namespace SangoC::Robotic::Sentry
{
	struct ComplexFireByte
	{
		unsigned char FireFlag : 2 = 0;
		unsigned char HoldFlag : 2 = 0;
		unsigned char FireFrequency : 4 = 0;

		ComplexFireByte();

		ComplexFireByte(
			const unsigned char fireFlag,
			const unsigned char holdFlag,
			const unsigned char fireFrequency
		);

		ComplexFireByte(const ComplexFireByte& other);
		ComplexFireByte(ComplexFireByte&& other);
		ComplexFireByte& operator=(const ComplexFireByte& other);
		ComplexFireByte& operator=(ComplexFireByte&& other);
	};
}
