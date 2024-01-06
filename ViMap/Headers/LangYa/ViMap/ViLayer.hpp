#pragma once

#include <vector>

#include <spdlog/spdlog.h>

#include <LangYa/Common/IFormatByStream.hpp>

#include <LangYa/TinyMath/Position.hpp>

#include <LangYa/ViMap/ViLayerSize.hpp>

namespace LangYa::ViMap
{
	class ViLayer final : public Common::IFormatByStream
	{
		/// @brief 该层的大小。
		ViLayerSize Size{};

		/// @brief 该层的代价数据。
		std::vector<float> CostData{};

	public:
		ViLayer();

		explicit ViLayer(ViLayerSize size, const float initialCost = 0);

		/// @brief 复制构造函数。
		ViLayer(const ViLayer& other);

		ViLayer& MergeLayerWithSameShape(const ViLayer& modification);

		ViLayer& MergePixel(const TinyMath::Position2Ui& pixelPosition, const float cost);

		float& operator[](const TinyMath::Position2Ui& pixelPosition);

		const float& operator[](const TinyMath::Position2Ui& pixelPosition) const;

		std::ostream& FormatByStream(std::ostream& stream, const int option = 0) const override;
	};
}
