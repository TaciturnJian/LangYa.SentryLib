#pragma once

#include <LangYa/SentryLib/Vector.hpp>
#include <LangYa/SentryLib/Position.hpp>

namespace LangYa::SentryLib {
	class ConsoleCursor {
		ConsoleCursor() = default;
	public:
		static ConsoleCursor& GetInstance();
		ConsoleCursor& Offset(const Vector2S& offset);
		ConsoleCursor& MoveTo(const Position2Us& position);
		ConsoleCursor& Show();
		ConsoleCursor& Hide();
	};
}