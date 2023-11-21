#pragma once

namespace SangoC::Memory::Resources
{
	enum class ResourceFlag : char
	{
		Busy = -1,
		Empty = 0,
		Available = 1
	};
}
