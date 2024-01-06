#ifdef SC_LINUX
#include <iostream>
#include <sstream>
#endif

#ifdef SC_WINDOWS
#include <Windows.h>
#endif

#include <LangYa/ConsoleHelper/ConsoleCursor.hpp>

namespace LangYa::ConsoleHelper
{
	ConsoleCursor& ConsoleCursor::GetInstance()
	{
		static ConsoleCursor cursor{};
		return cursor;
	}

	ConsoleCursor& ConsoleCursor::Offset(const TinyMath::Vector2S& offset)
	{
#ifdef SC_LINUX
		if (offset[0] == offset[1] && offset[0] == 0) return *this;

		std::stringstream stream;

		if (const auto y = offset[1]; y < 0) 
			stream << "\033[" << -y << 'A';
		else if (y > 0) stream << "\033[" << y << 'B';

		if (const auto x = offset[0]; x < 0) 
			stream << "\033[" << -x << 'D';
		else if (x > 0) stream << "\033[" << x << 'C';

		std::cout << stream.str();
		return *this;
#endif

#ifdef SC_WINDOWS
		if (offset[0] == offset[1] && offset[0] == 0) return *this;

		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		info.dwCursorPosition.Y += static_cast<short>(offset[1]);     // NOLINT(bugprone-narrowing-conversions)
		if (info.dwCursorPosition.Y < 0) info.dwCursorPosition.Y = 0; // NOLINT(bugprone-narrowing-conversions)

		info.dwCursorPosition.X += static_cast<short>(offset[0]);
		if (info.dwCursorPosition.X < 0) info.dwCursorPosition.X = 0;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), info.dwCursorPosition);
		return *this;
#endif
	}

	ConsoleCursor& ConsoleCursor::MoveTo(const TinyMath::Position2Us& position)
	{
#ifdef SC_LINUX
		std::stringstream stream;
		stream << "\033[" << position.X << ';' << position.Y << 'H';
		std::cout << stream.str();
		return *this;
#endif

#ifdef SC_WINDOWS
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		info.dwCursorPosition.X = static_cast<short>(position.X);
		info.dwCursorPosition.Y = static_cast<short>(position.Y);

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), info.dwCursorPosition);
		return *this;
#endif
	}

	ConsoleCursor& ConsoleCursor::Show()
	{
#ifdef SC_LINUX
		std::cout << "\033[?25h";
		return *this;
#endif

#ifdef SC_WINDOWS
		const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursor_info;
		GetConsoleCursorInfo(handle, &cursor_info);
		cursor_info.bVisible = true;
		SetConsoleCursorInfo(handle, &cursor_info);
		return *this;
#endif
	}

	ConsoleCursor& ConsoleCursor::Hide()
	{
#ifdef SC_LINUX
		std::cout << "\033[?25l";
		return *this;
#endif

		#ifdef SC_WINDOWS
		const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursor_info;
		GetConsoleCursorInfo(handle, &cursor_info);
		cursor_info.bVisible = false;
		SetConsoleCursorInfo(handle, &cursor_info);
		return *this;
#endif
	}
}
