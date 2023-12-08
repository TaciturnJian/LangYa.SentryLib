#pragma once

#include <LangYa/SentryLib/Vector2.hpp>

namespace LangYa::SentryLib {
	class ConsoleManipulator 
	{
	public:
		enum CommonColor : char {
			Black = 0,
			Red = 1,
			Green = 2,
			Yellow = 3,
			Blue = 4,
			Magenta = 5,
			Cyan = 6,
			White = 7
		};

		ConsoleManipulator& ResetEffect();
		ConsoleManipulator& TurnOnBoldText();
		ConsoleManipulator& TurnOffBoldText();
		ConsoleManipulator& TurnOnUnderlineText();
		ConsoleManipulator& TurnOffUnderlineText();
		ConsoleManipulator& TurnOnBlinkText();
		ConsoleManipulator& TurnOffBlinkText();
		ConsoleManipulator& TurnOnInverseColor();
		ConsoleManipulator& SetForegroundColor(CommonColor color);
		ConsoleManipulator& SetBackgroundColor(CommonColor color);
		ConsoleManipulator& MoveCursor(Vector2S offset);
		ConsoleManipulator& MoveCursorTo(Position2Ui position);
		ConsoleManipulator& ClearScreen();
		ConsoleManipulator& ClearToLineEnd();
		ConsoleManipulator& SaveCursorPosition();
		ConsoleManipulator& ReturnToLastSavedCursorPosition();
		ConsoleManipulator& HideCursor();
		ConsoleManipulator& ShowCursor();
	};
}
