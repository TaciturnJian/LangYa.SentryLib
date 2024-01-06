#pragma once

namespace LangYa::Common
{
	/// @brief 代表资源的状态。
	enum class ResourceFlag : char
	{
		/// @brief Indicate the resource is in busy status, which means you cannot read or write this now.
		Busy = -1,

		/// @brief Indicate the resource is empty, which means you can write now.
		Empty = 0,

		/// @brief Indicate the resource is available, which means you can read now.
		Available = 1
	};
}
