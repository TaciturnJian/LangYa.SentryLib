#include <iostream>
#include <sstream>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/MemoryView.hpp>

using namespace LangYa::SentryLib;

int main()
{
	spdlog::info("<MemoryViewTester>");

	spdlog::info("Simple memory access test, expected: bytes of [1,2,3,4,5] (5 * sizeof(int) bytes)");
	{
		int a[5]{1, 2, 3, 4, 5};
		const MemoryView view{a, 5};
		std::stringstream temp_stream;
		temp_stream << view;
		spdlog::info(temp_stream.str());
	}

	spdlog::info("</MemoryViewTester>");
}
