#pragma once

#include <LangYa/SentryLib/Math/Vector3.hpp>
#include <LangYa/SentryLib/Math/Position3.hpp>

namespace LangYa::SentryLib {
	/// @brief ArmorPlate 装甲板信息，储存装甲板的位置、法向量、颜色、ID。
	///	@details 装甲板是视觉识别的主要对象。
	///	视觉识别到的装甲板通常会带有相对世界位置、法向量、装甲板颜色、装甲板ID四大数据。
	///	使用通用的装甲板避免歧义。
	class ArmorPlate {
	public:
		/// @brief 三维 float 位置。
		Position3F Position{};

		/// @brief 三维 float 法向量，其模不一定为 1，此向量仅代表装甲板方向。
		Vector3F NormalDirection{};

		/// @brief 装甲板上 LED 的色彩。
		///	@TODO 使用标准的 enum 代表颜色。
		int Color{};

		/// @brief 装甲板的 ID。
		///	@TODO 使用标准的 enum 代表 ID。
		int ID{};
	};
}