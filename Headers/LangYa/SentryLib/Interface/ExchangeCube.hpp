#pragma once

#include <LangYa/SentryLib/Math/Position3.hpp>
#include <LangYa/SentryLib/Math/Vector.hpp>

namespace LangYa::SentryLib {
	/// @brief 代表兑换框，避免歧义
	///	@details 兑换框是工程机器人视觉识别的重要目标，此类用于表示兑换框的位置和朝向。
	class ExchangeCube {
	public:
		Vector3F Position{};
		Position3F NormalDirection{};
	};
}
