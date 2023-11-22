#pragma once

namespace LangYa::Sentry
{
	/// @brief Represents the status of a resource.
	enum class ResourceFlag : char
	{
		Busy = -1,
		Empty = 0,
		Available = 1
	};
}
