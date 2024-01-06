#include <LangYa/ViMap/ViLayer.hpp>

namespace LangYa::ViMap
{
	ViLayer
	::ViLayer()
	= default;

	ViLayer
	::ViLayer(ViLayerSize size, const float initialCost):
		Size(std::move(size))
	{
		const auto& layer_size = Size.LayerSize;
		CostData.resize(static_cast<std::uint64_t>(layer_size.Width) * layer_size.Height, initialCost);
		if (CostData.empty())
		{
			spdlog::warn("ViLayer> Constructed a layer with specific size, but none of elements found");
		}
	}

	ViLayer
	::ViLayer(const ViLayer& other)
	= default;

	ViLayer&
	ViLayer
	::MergeLayerWithSameShape(const ViLayer& modification)
	{
		if (CostData.size() != modification.CostData.size())
		{
			throw std::invalid_argument("ViLayer> MergeLayerWithSameShape: The layer to merge has different shape");
		}

		for (std::uint64_t i = 0; i < CostData.size(); ++i)
		{
			CostData[i] += modification.CostData[i];
		}
		return *this;
	}

	ViLayer&
	ViLayer
	::MergePixel(const TinyMath::Position2Ui& pixelPosition, const float cost)
	{
		if (!Size.ContainsPixel(pixelPosition))
		{
			spdlog::warn("ViLayer> Trying to merge cost pixel that is out of the layer, ignored this merge");
			return *this;
		}

		CostData[Size.GetIndexOfPixel(pixelPosition)] += cost;
		return *this;
	}

	float&
	ViLayer
	::operator[](const TinyMath::Position2Ui& pixelPosition)
	{
		if (!Size.ContainsPixel(pixelPosition))
		{
			throw std::invalid_argument("position is out of the layer");
		}

		return CostData[Size.GetIndexOfPixel(pixelPosition)];
	}

	const float&
	ViLayer
	::operator[](const TinyMath::Position2Ui& pixelPosition) const
	{
		if (!Size.ContainsPixel(pixelPosition))
		{
			throw std::invalid_argument("position is out of the layer");
		}

		return CostData[Size.GetIndexOfPixel(pixelPosition)];
	}

	std::ostream&
	ViLayer
	::FormatByStream(std::ostream& stream, const int option) const
	{
		if (option == 0) // 控制台友好型，先输出尺寸，再输出像素代价
		{
			stream << "Size:" << Size << "CostsList:[";
			if (!CostData.empty())
			{
				for (std::uint64_t i = 0; i < CostData.size() - 1; ++i)
				{
					stream << CostData[i] << ',';
				}
				stream << CostData[CostData.size() - 1];
			}

			return stream << ']';
		}

		spdlog::warn("ViLayer> Unknown option for formatting this layer, trying with default option");

		return FormatByStream(stream);
	}
}
