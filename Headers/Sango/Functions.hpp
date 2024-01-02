#pragma once

#include <functional>

namespace Sango {
	template<typename TActionTarget>
	using ActionType = std::function<void(TActionTarget)>;

	using TaskType = std::function<void()>;
}