#pragma once

#include <LangYa/SentryLib/Position3.hpp>
#include <LangYa/SentryLib/Vector3.hpp>

namespace LangYa::SentryLib {
	/// @brief 代表一个装甲板的信息。
	///	内部使用 Position3F 表示位置，使用 Vector3F 表示法向量。
	struct ArmorPlateInfo {
		/// @brief 装甲板的位置
		/// 可能是相对世界，也可能是相对于自己，请注意代码上下文。
		Position3F Position{};

		/// @brief 装甲板的“法”向量。
		///	此向量的长度可能不是精确的 1，但是方向一定是正确表示的。
		Vector3F Normal{};
	};
}