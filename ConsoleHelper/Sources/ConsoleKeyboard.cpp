#include <LangYa/ConsoleHelper/ConsoleKeyboard.hpp>

#ifdef SC_WINDOWS
#include <conio.h>

#define GET_CHAR_FROM_KEYBOARD _getch
#endif

#ifdef SC_LINUX
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <unistd.h>
#include <sys/ioctl.h>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;
 
    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }
 
    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
#define GET_CHAR_FROM_KEYBOARD std::getchar

#endif

#define CHECK_KEYBOARD_HIT _kbhit

namespace LangYa::ConsoleHelper {
	ConsoleKeyboard& 
	ConsoleKeyboard::GetInstance()
	{
		static ConsoleKeyboard keyboard{};
#ifdef LINUX
		static bool initialized{false};
		if (!initialized) {
			tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
			t.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
			tcsetattr(STDIN_FILENO, TSCANOW, &t); //Apply the new settings
		}
#endif

		return keyboard;
	}

	ConsoleKeyboard& 
	ConsoleKeyboard::GetKey(char& key)
	{
		if (!CHECK_KEYBOARD_HIT())
		{
			key = 0;
			return *this;
		}
		key = static_cast<char>(GET_CHAR_FROM_KEYBOARD());
		return *this;
	}

	ConsoleKeyboard& 
	ConsoleKeyboard
	::GetKeyboardHit(bool& hit)
	{
		hit = CHECK_KEYBOARD_HIT();
		return *this;
	}
}
