#pragma once

#include <ostream>
#include <istream>
#include <unordered_map>

#include <LangYa/SentryLib/Position.hpp>
#include <LangYa/SentryLib/Vector.hpp>

namespace LangYa::SentryLib
{
	class ViMap 
	{
	public:
		using CostType = unsigned int;
		using PixelListType = std::unordered_map<Position2I, CostType>;
		using PointListType = std::vector<Position2I>;

		CostType GetCost(const Position2I& pixelPosition, const Vector2I& direction);
		CostType CalculateCost(const Vector2I& direction);
		ViMap MergePixelList(const PixelListType& list);

		struct PointListSampleOption {};
		static PointListType SamplePoints(const PointListType& points, const PointListSampleOption& option);
		friend std::ostream& operator<<(std::ostream& stream, const PointListType& list);
		friend std::istream& operator>>(std::istream& stream, PointListType& list);

		struct PixelListSampleOption {};
		static PixelListType SamplePixels(const PixelListType& pixels, const PixelListSampleOption& option);
		friend std::ostream& operator<<(std::ostream& stream, const PixelListType& list);
		friend std::istream& operator>>(std::istream& stream, PixelListType& list);
	};
}