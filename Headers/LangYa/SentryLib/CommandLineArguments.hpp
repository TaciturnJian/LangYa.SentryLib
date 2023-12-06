#pragma once

namespace LangYa::SentryLib
{
	/// @brief 记录控制台参数。
	struct CommandLineArguments
	{
		using CommandLineArgumentType = const char*;
		using CommandLineArgumentListType = const CommandLineArgumentType*;

		/// @brief 控制台参数的数量。
		unsigned int ArgumentCount{0};

		/// @brief 控制台参数的列表，注意是二级指针。
		CommandLineArgumentListType ArgumentList{nullptr};

		CommandLineArguments();
		CommandLineArguments(int count, CommandLineArgumentListType list);
		CommandLineArguments(unsigned int count, CommandLineArgumentListType list);
		CommandLineArguments(const CommandLineArguments& other);
		CommandLineArguments(CommandLineArguments&& other) noexcept;
		CommandLineArguments& operator=(const CommandLineArguments& other);
		CommandLineArguments& operator=(CommandLineArguments&& other) noexcept;

		/// @brief 访问控制台参数。
		CommandLineArgumentType operator[](unsigned int index) const;
	};
}
