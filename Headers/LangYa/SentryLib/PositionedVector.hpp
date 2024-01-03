#pragma once

#include <LangYa/SentryLib/IFormatByStream.hpp>
#include <LangYa/SentryLib/Position.hpp>
#include <LangYa/SentryLib/Vector.hpp>

namespace LangYa::SentryLib {
	template<typename TElement>
	struct PositionedVector3 final : IFormatByStream {
		Position3<TElement> Position{};
		Vector3<TElement> Direction{};

		std::ostream& FormatByStream(std::ostream& stream, int option = 0) const override
		{
			stream << "{";
			stream << R"("Position":)";
			Position.FormatByStream(stream);
			stream << ",";
			stream << R"("Direction":)";
			Direction.FormatByStream(stream);
			stream << "}";
			return stream;
		}
	};

	using PositionedVector3I = PositionedVector3<int>;
	using PositionedVector3F = PositionedVector3<float>;
	using PositionedVector3D = PositionedVector3<double>;

	template<typename TElement>
	struct PositionedVector2 : IFormatByStream {
		Position2<TElement> Position{};
		Vector2<TElement> Direction{};

		std::ostream& FormatByStream(std::ostream& stream, int option) const override
		{
			stream << "{";
			stream << R"("Position":)";
			Position.FormatByStream(stream);
			stream << ",";
			stream << R"("Direction":)";
			Direction.FormatByStream(stream);
			stream << "}";
			return stream;
		}
	};

	using PositionedVector2I = PositionedVector2<int>;
	using PositionedVector2F = PositionedVector2<float>;
	using PositionedVector2D = PositionedVector2<double>;
}