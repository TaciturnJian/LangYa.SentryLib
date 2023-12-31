#include <iomanip>
#include <iostream>
#include <thread>

#include <LangYa/SentryLib/ConsoleManipulator.hpp>

using namespace LangYa::SentryLib;
using namespace std::chrono_literals;

int main()
{
	ConsoleManipulator console{};
	console.ClearScreen().HideCursor();

	for (int i = 0; i < 1000; i ++)
	{
		std::cout << "[1]: A-> " << std::setw(5) << i << '\n';
		std::cout << "[2]: B-> " << std::setw(3) << i * 317 % 1000 << '\n';
		console.MoveCursorTo({0,0});
		std::this_thread::sleep_for(10ms);
	}
}
