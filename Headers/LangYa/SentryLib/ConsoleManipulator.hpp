#pragma once

#include <LangYa/SentryLib/Math/Vector.hpp>
#include <LangYa/SentryLib/Math/Position2.hpp>

namespace LangYa::SentryLib {
	/// @brief 控制台操作器
	///	封装了许多控制台的常用操作。
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
		ConsoleManipulator& MoveCursor(const Vector2S& offset);
		ConsoleManipulator& MoveCursorTo(const Position2Ui& position);
		ConsoleManipulator& ClearScreen();
		ConsoleManipulator& ClearToLineEnd();
		ConsoleManipulator& SaveCursorPosition();
		ConsoleManipulator& ReturnToLastSavedCursorPosition();
		ConsoleManipulator& HideCursor();
		ConsoleManipulator& ShowCursor();
	};
}
