#pragma once

#include <LangYa/SentryLib/PositionedVector.hpp>
#include <LangYa/SentryLib/Interface/TeamArmorColor.h>

#include <LangYa/SentryLib/IFormatByStream.hpp>

namespace LangYa::SentryLib
{
	/// @brief ArmorPlate 装甲板信息，储存装甲板的位置、法向量、颜色、ID。
	///	@details 装甲板是视觉识别的主要对象。
	///	视觉识别到的装甲板通常会带有相对世界位置、法向量、装甲板颜色、装甲板ID四大数据。
	///	使用通用的装甲板避免歧义。
	class ArmorPlate final : public IFormatByStream
	{
	public:
		/// @brief 装甲板上图案对应的数字 ID。
		char ID{};

		/// @brief 装甲板上 LED 的色彩。
		ArmorColor Color{};

		/// @brief 装甲板中心质点，包含位置和法向量。
		PositionedVector3F Particle{};

		ArmorPlate();

		ArmorPlate(char id, ArmorColor color, PositionedVector3F particle);

		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override;
	};
}
