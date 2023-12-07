#pragma once

/// @brief 代表一个类似 C# 接口。
///	严格来说，C++并不存在接口，但是可以通过抽象类来模拟接口。
///	所以被此宏标记的类必须是一个抽象类。
#define SC_INTERFACE class

/// @brief 程序的入口点。
#define SC_ENTRY_POINT int main(const int argc, const char* argv[])
