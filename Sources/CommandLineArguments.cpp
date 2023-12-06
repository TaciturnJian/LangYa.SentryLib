#include <LangYa/SentryLib/CommandLineArguments.hpp>

namespace LangYa::SentryLib
{
	CommandLineArguments
	::CommandLineArguments() = default;

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

	CommandLineArguments
	::CommandLineArguments(const CommandLineArguments& other)
	= default;

	CommandLineArguments
	::CommandLineArguments(CommandLineArguments&& other) noexcept
	= default;

	CommandLineArguments&
	CommandLineArguments
	::operator=(const CommandLineArguments& other)
	= default;

	CommandLineArguments&
	CommandLineArguments
	::operator=(CommandLineArguments&& other) noexcept
	= default;

	CommandLineArguments::CommandLineArgumentType
	CommandLineArguments
	::operator[](const unsigned index) const
	{
		if (index >= ArgumentCount)
		{
			return nullptr;
		}

		return ArgumentList[index];
	}
}
