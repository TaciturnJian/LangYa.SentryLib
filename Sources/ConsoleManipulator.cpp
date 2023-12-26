#ifdef SC_LINUX
#include <iostream>
#include <sstream>
#include <string>
#endif

#ifdef SC_WINDOWS
#include <Windows.h>
#endif
                                              
#include <LangYa/SentryLib/ConsoleManipulator.hpp>

namespace LangYa::SentryLib
{
#ifdef SC_LINUX

#define SC_ONE_LINE_COMMAND(x) {std::cout << (x); return *this;}

	ConsoleManipulator&
	ConsoleManipulator
	::ResetEffect()
	SC_ONE_LINE_COMMAND("\033[0m")

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnBoldText()
	SC_ONE_LINE_COMMAND("\033[1m")

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOffBoldText()
	SC_ONE_LINE_COMMAND("\033[22m")

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnUnderlineText()
	SC_ONE_LINE_COMMAND("\033[4m")

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOffUnderlineText()
	SC_ONE_LINE_COMMAND("\033[24m")

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnBlinkText()
	SC_ONE_LINE_COMMAND("\033[5m")

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOffBlinkText()
	SC_ONE_LINE_COMMAND("\033[25m")

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnInverseColor()
	SC_ONE_LINE_COMMAND("\033[7m")

	ConsoleManipulator&
	ConsoleManipulator
	::SetForegroundColor(const CommonColor color)
	{
		std::stringstream stream;
		stream << "\033[3" << '0' + static_cast<char>(color) << "m";
		std::cout << stream.str();
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::SetBackgroundColor(const CommonColor color)
	{
		std::stringstream stream;
		stream << "\033[4" << '0' + static_cast<char>(color) << "m";
		std::cout << stream.str();
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::MoveCursor(const Vector2S offset)
	{
		if (offset[0] == offset[1] && offset[0] == 0) return *this;

		std::stringstream stream;

		if (const auto y = offset[1]; y < 0)
		{
			stream << "\033[" << -y << 'A';
		}
		else if (y > 0)
		{
			stream << "\033[" << y << 'B';
		}

		if (const auto x = offset[0]; x < 0)
		{
			stream << "\033[" << -x << 'D';
		}
		else if (x > 0)
		{
			stream << "\033[" << x << 'C';
		}

		std::cout << stream.str();
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::MoveCursorTo(const Position2Ui& position)
	{
		std::stringstream stream;
		stream << "\033[" << position.X << ';' << position.Y << 'H';
		std::cout << stream.str();
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::ClearScreen()
	SC_ONE_LINE_COMMAND("\033[2J")

	ConsoleManipulator&
	ConsoleManipulator
	::ClearToLineEnd()
	SC_ONE_LINE_COMMAND("\033[K")

	ConsoleManipulator&
	ConsoleManipulator
	::SaveCursorPosition()
	SC_ONE_LINE_COMMAND("\033[s")

	ConsoleManipulator&
	ConsoleManipulator
	::ReturnToLastSavedCursorPosition()
	SC_ONE_LINE_COMMAND("\033[u")

	ConsoleManipulator&
	ConsoleManipulator
	::HideCursor()
	SC_ONE_LINE_COMMAND("\033[?25l")

	ConsoleManipulator&
	ConsoleManipulator
	::ShowCursor()
	SC_ONE_LINE_COMMAND("\033[?25h")

#undef SC_ONE_LINE_COMMAND

#endif

#ifdef SC_WINDOWS
	ConsoleManipulator&
	ConsoleManipulator
	::ResetEffect()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnBoldText()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOffBoldText()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnUnderlineText()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_UNDERSCORE
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOffUnderlineText()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnBlinkText()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_REVERSE_VIDEO
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOffBlinkText()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::TurnOnInverseColor()
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | COMMON_LVB_REVERSE_VIDEO
		);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::SetForegroundColor(const CommonColor color)
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | static_cast<WORD>(color));
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::SetBackgroundColor(const CommonColor color)
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			BACKGROUND_INTENSITY | static_cast<WORD>(color));
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::MoveCursor(const Vector2S& offset)
	{
		if (offset[0] == offset[1] && offset[0] == 0) return *this;

		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		info.dwCursorPosition.Y += static_cast<short>(offset[1]);     // NOLINT(bugprone-narrowing-conversions)
		if (info.dwCursorPosition.Y < 0) info.dwCursorPosition.Y = 0; // NOLINT(bugprone-narrowing-conversions)

		info.dwCursorPosition.X += static_cast<short>(offset[0]);
		if (info.dwCursorPosition.X < 0) info.dwCursorPosition.X = 0;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), info.dwCursorPosition);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator::MoveCursorTo(const Position2Ui& position)
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		info.dwCursorPosition.X = static_cast<short>(position.X);
		info.dwCursorPosition.Y = static_cast<short>(position.Y);

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), info.dwCursorPosition);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::ClearScreen()
	{
		system("cls"); // NOLINT(concurrency-mt-unsafe)
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::ClearToLineEnd()
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		const auto length = info.dwSize.X - info.dwCursorPosition.X;
		DWORD written;
		FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', length, info.dwCursorPosition, &written);
		FillConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), info.wAttributes, length, info.dwCursorPosition,
		                           &written);
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::SaveCursorPosition()
	{
		//TODO 未实现
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator::ReturnToLastSavedCursorPosition()
	{
		//TODO 未实现
		return *this;
	}

	ConsoleManipulator&
	ConsoleManipulator
	::HideCursor()
	{
		const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursor_info;
		GetConsoleCursorInfo(handle, &cursor_info);
		cursor_info.bVisible = false;
		SetConsoleCursorInfo(handle, &cursor_info);
		return *this;
	}

	ConsoleManipulator& ConsoleManipulator::ShowCursor()
	{
		const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursor_info;
		GetConsoleCursorInfo(handle, &cursor_info);
		cursor_info.bVisible = true;
		SetConsoleCursorInfo(handle, &cursor_info);
		return *this;
	}

#endif
}
