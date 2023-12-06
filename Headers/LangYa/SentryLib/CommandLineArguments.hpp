#pragma once

namespace LangYa::SentryLib
{
	struct CommandLineArguments
	{
		using CommandLineArgumentType = const char*;
		using CommandLineArgumentListType = const CommandLineArgumentType*;

		unsigned int ArgumentCount{0};
		CommandLineArgumentListType ArgumentList{nullptr};

		CommandLineArguments();
		CommandLineArguments(int count, CommandLineArgumentListType list);
		CommandLineArguments(unsigned int count, CommandLineArgumentListType list);
		CommandLineArguments(const CommandLineArguments& other);
		CommandLineArguments(CommandLineArguments&& other) noexcept;
		CommandLineArguments& operator=(const CommandLineArguments& other);
		CommandLineArguments& operator=(CommandLineArguments&& other) noexcept;

		CommandLineArgumentType operator[](unsigned int index) const;
	};
}
