#include <iostream>

#include <LangYa/SentryLib/MemoryView.hpp>

using namespace LangYa::SentryLib;

namespace UseMemoryView
{
	void WrapArrayAndDisplayItsBytes()
	{
		int a[5]{1, 2, 3, 4, 5};
		std::cout << MemoryView{a, 5};
	}

	void SeparateBufferManipulationToOneModifyAndOneView()
	{
		char buffer[10]{0};
		MemoryView shared_buffer_view{buffer, 10};

		auto writer = [&buffer]
		{
			buffer[0] = '!';
		};

		auto viewer = [shared_buffer_view]
		{
			std::cout << shared_buffer_view[0];
		};

		writer();
		viewer();
	}
} 

int main()
{
	UseMemoryView::WrapArrayAndDisplayItsBytes();
	UseMemoryView::SeparateBufferManipulationToOneModifyAndOneView();
	return 0;
}
