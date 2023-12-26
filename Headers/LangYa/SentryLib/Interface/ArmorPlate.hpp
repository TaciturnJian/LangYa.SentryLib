#pragma once

#include <LangYa/SentryLib/Math/Vector.hpp>
#include <LangYa/SentryLib/Math/Position3.hpp>
#include <LangYa/SentryLib/Interface/TeamArmorColor.h>

namespace LangYa::SentryLib
{
	/// @brief ArmorPlate 装甲板信息，储存装甲板的位置、法向量、颜色、ID。
	///	@details 装甲板是视觉识别的主要对象。
	///	视觉识别到的装甲板通常会带有相对世界位置、法向量、装甲板颜色、装甲板ID四大数据。
	///	使用通用的装甲板避免歧义。
	class ArmorPlate : public CanStreamFormatToJson
	{
	public:
		/// @brief 装甲板上图案对应的数字 ID。
		char ID{};

		/// @brief 装甲板上 LED 的色彩。
		ArmorColor Color{};

		/// @brief 三维 float 位置。
		Position3F Position{};

		/// @brief 三维 float 法向量，其模不一定为 1，此向量仅代表装甲板方向。
		Vector3F NormalDirection{};

		ArmorPlate() = default;

		ArmorPlate(const char id, const ArmorColor color, Position3F position, const Vector3F& normalDirection) :
			ID(id), Color(color), Position(std::move(position)), NormalDirection(normalDirection)
		{
		}

		std::ostream& FormatToJson(std::ostream& stream) const override
		{
			return stream << "{"
				<< "\"ID\":" << static_cast<int>(ID) << ','
				<< "\"Color\":" << static_cast<int>(Color) << ','
				<< "\"Position\":" << Position << ','
				<< "\"NormalDirection\":" << NormalDirection
				<< "}";
		}
	};
}
