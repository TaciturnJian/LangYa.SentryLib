#pragma once

namespace LangYa::ConsoleHelper {
	class ConsoleKeyboard {
		ConsoleKeyboard() = default;

	public:
		static ConsoleKeyboard& GetInstance();

		ConsoleKeyboard& GetKey(char& key);

		ConsoleKeyboard& GetKeyboardHit(bool& hit);
	};
}