#pragma once

#include <LangYa/Common/IFormatByStream.hpp>

#include <LangYa/TinyMath/RectangularSize.hpp>
#include <LangYa/TinyMath/Position.hpp>

namespace LangYa::ViMap
{
	struct ViLayerSize final : Common::IFormatByStream
	{
		/// @brief 层的长宽像素个数。
		TinyMath::RectangularSizeU32 LayerSize{};

		/// @brief 层的像素实际大小。（物理世界的单位需要自己确定）
		TinyMath::RectangularSizeF PixelRealSize{};

		ViLayerSize();

		ViLayerSize(TinyMath::RectangularSizeU32 layerSize, TinyMath::RectangularSizeF pixelRealSize);

		[[nodiscard]] bool ContainsPixel(const TinyMath::Position2F& realPosition) const;

		[[nodiscard]] bool ContainsPixel(const TinyMath::Position2Ui& pixelPosition) const;

		[[nodiscard]] std::uint64_t GetIndexOfPixel(const TinyMath::Position2Ui& pixelPosition) const;

		friend bool operator==(const ViLayerSize& left, const ViLayerSize& right);

		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override;
	};
}
