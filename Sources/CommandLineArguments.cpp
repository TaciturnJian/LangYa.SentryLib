#include <LangYa/SentryLib/Application/CommandLineArguments.hpp>

/*
	反正不会有人看这里
	 <(￣︶￣)>
*/

namespace LangYa::SentryLib
{
	CommandLineArguments
	::CommandLineArguments()
	= default;

	CommandLineArguments
	::CommandLineArguments(const int count, const CommandLineArgumentListType list) :
		ArgumentCount(count),
		ArgumentList(list)
	{
	}

	/* 理论上，上下两种格式都可以接收，但是编译器对 signed 和 unsigned 的区分很严格，所以默认都改成 unsigned */

	CommandLineArguments
	::CommandLineArguments(const unsigned count, const CommandLineArgumentListType list) :
		ArgumentCount(count),
		ArgumentList(list)
	{
	}

	CommandLineArguments::CommandLineArgumentType
	CommandLineArguments
	::operator[](const unsigned index) const noexcept
	{
		/* 实话说，不用 if 而改用三元表达式就是有病，但是我有病 awa */
		return index >= ArgumentCount ? nullptr : ArgumentList[index];
	}
}
