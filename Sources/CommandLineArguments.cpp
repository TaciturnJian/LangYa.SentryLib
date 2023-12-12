#include <LangYa/SentryLib/Application/CommandLineArguments.hpp>

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
		if (index >= ArgumentCount)
		{
			return nullptr;
		}

		return ArgumentList[index];
	}
}
