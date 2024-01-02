#include <iostream>
#include <locale>
#include <fstream>
#include <thread>

struct ConsoleKeyInfo
{
	/// @brief 控制台按键的名称，用来于其他按键区分。
	const char *Name;

	/// @brief 控制台按键的ID，用于快速查询按键名称等。
	unsigned char KeyID{};

	/// @brief 按键表示的字符，如果是
	unsigned char KeyChar{};
};

class ConsoleKey
{
public:
	virtual const char* GetName() const;
	virtual int GetID() const;
};

class ConsoleKeyboard
{
	ConsoleKeyboard() = default;
public:
	~ConsoleKeyboard() = default;

	static ConsoleKeyboard& GetInstance()
	{
		static ConsoleKeyboard instance;
		return instance;
	}


};

int main()
{
	



	return 0;
}
