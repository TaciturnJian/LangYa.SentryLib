#pragma once

#include <LangYa/SentryLib/Math/Position3.hpp>
#include <LangYa/SentryLib/Math/Vector3.hpp>
#include <LangYa/SentryLib/RoboMaster.hpp>
#include <LangYa/SentryLib/TimeStamp.hpp>

namespace LangYa::SentryLib {
	/// @brief 代表一个装甲板的信息。
	///	内部使用 Position3F 表示位置，使用 Vector3F 表示法向量。
	struct ArmorPlateInfo : TimeStamp
	{
		/// @brief 装甲板的位置
		/// 可能是相对世界，也可能是相对于自己，请注意代码上下文。
		Position3F Position{};

		/// @brief 装甲板的“法”向量。
		///	此向量的长度可能不是精确的 1，但是方向一定是正确表示的。
		Vector3F Normal{};

		/// @brief 此装甲板对应的团队。
		///	注意，中立团队一般代表错误或未知的装甲板。
		UnitTeam Team{UnitTeam::Neutral};

		/// @brief 此装甲板对应的机器人ID。
		/// 如果
		UnitID ID{};

		/// @brief 此装甲板对应的机器人类型。
		UnitType Type{};

		/// @brief 默认的构造函数
		ArmorPlateInfo() = default;
	};
}