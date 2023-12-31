#pragma once

#include <LangYa/TinyMath/Position.hpp>
#include <LangYa/TinyMath/Vector.hpp>

namespace LangYa::SentryLib {
	/// @brief 代表兑换框，避免歧义。
	///	@details 兑换框是工程机器人视觉识别的重要目标，此类用于表示兑换框的位置和朝向。
	class ExchangeCube {
	public:
		/// @brief 兑换框的位置。dsadsa
		TinyMath::Vector3F Position{};

		/// @brief 兑换框的朝向。
		TinyMath::Position3F NormalDirection{};
	};
}
