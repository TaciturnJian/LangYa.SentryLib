#include <iostream>

#include <LangYa/SentryLib/Math/Position3.hpp>

#include <LangYa/SentryLib/Math/Vector.hpp>

int main() {
	using namespace LangYa::SentryLib;

	const Vector2I p1{1,2}, p2{3,4};

	std::cout << "Now print the json format of points: \n";
	std::cout << p1 + p2 << '\n' << p1 - p2 <<  '\n';
	return 0;
}
