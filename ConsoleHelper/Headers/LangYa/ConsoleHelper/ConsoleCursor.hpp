#pragma once

#include <LangYa/TinyMath/Vector.hpp>
#include <LangYa/TinyMath/Position.hpp>

namespace LangYa::ConsoleHelper {
	class ConsoleCursor {
		ConsoleCursor() = default;
	public:
		static ConsoleCursor& GetInstance();
		ConsoleCursor& Offset(const TinyMath::Vector2S& offset);
		ConsoleCursor& MoveTo(const TinyMath::Position2Us& position);
		ConsoleCursor& Show();
		ConsoleCursor& Hide();
	};
}