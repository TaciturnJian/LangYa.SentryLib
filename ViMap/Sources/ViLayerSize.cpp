#include <LangYa/ViMap/ViLayerSize.hpp>

namespace LangYa::ViMap
{
	using namespace TinyMath;

	ViLayerSize
	::ViLayerSize()
	= default;

	ViLayerSize
	::ViLayerSize(RectangularSizeU32 layerSize, RectangularSizeF pixelRealSize):
		LayerSize(std::move(layerSize)),
		PixelRealSize(std::move(pixelRealSize))
	{
	}

	bool
	ViLayerSize
	::ContainsPixel(const Position2F& realPosition) const
	{
		const auto real_width = PixelRealSize.Width * static_cast<float>(LayerSize.Width);
		const auto real_height = PixelRealSize.Height * static_cast<float>(LayerSize.Height);
		return
			realPosition.X >= 0 && realPosition.X < real_width &&
			realPosition.Y >= 0 && realPosition.Y < real_height;
	}

	bool
	ViLayerSize
	::ContainsPixel(const Position2Ui& pixelPosition) const
	{
		return pixelPosition.X < LayerSize.Width && pixelPosition.Y < LayerSize.Height;
	}

	std::uint64_t
	ViLayerSize
	::GetIndexOfPixel(const Position2Ui& pixelPosition) const
	{
		return pixelPosition.Y * LayerSize.Width + pixelPosition.X;
	}

	bool
	operator==(const ViLayerSize& left, const ViLayerSize& right)
	{
		return left.LayerSize == right.LayerSize && left.PixelRealSize == right.PixelRealSize;
	}

	std::ostream&
	ViLayerSize
	::FormatByStream(std::ostream& stream, const int option) const
	{
		if (option == 0) // 控制台友好型，输出格式 (width height)*(pixelWidth pixelHeight)
		{
			return stream << '(' << LayerSize << ")*(" << PixelRealSize << ')';
		}

		if (option == 1) // json 紧凑格式输出，输出数组数组，格式为 [[width,height],[pixelWidth,pixelHeight]]
		{
			return PixelRealSize.FormatByStream(
				LayerSize.FormatByStream(
					stream << '['
				) << ','
			) << ']';
		}

		spdlog::warn("ViLayerSize> Unknown option for formatting this layer size, trying with default option");

		return FormatByStream(stream);
	}
}
