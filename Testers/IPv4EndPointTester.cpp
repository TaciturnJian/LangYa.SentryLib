#include <iostream>
#include <LangYa/SentryLib/IPv4Address.hpp>

int main()
{
	LangYa::SentryLib::IPv4Address address{};

	address = "192:168k137q";

	std::cout << LangYa::SentryLib::FormatToString(address);

	return 0;
}