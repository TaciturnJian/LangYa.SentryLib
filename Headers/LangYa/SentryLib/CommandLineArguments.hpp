#pragma once

namespace LangYa::SentryLib
{
	/// @brief 记录控制台参数。
	///	本质就是包装了一层的main函数的参数，方便使用。
	struct CommandLineArguments
	{
		/// @brief 控制台参数的类型 => 常量字符串。
		using CommandLineArgumentType = const char*;

		/// @brief 控制台参数的列表类型 => 常量字符串的二级指针。
		using CommandLineArgumentListType = const CommandLineArgumentType*;

		/// @brief 控制台参数的数量。
		unsigned int ArgumentCount{0};

		/// @brief 控制台参数的列表，注意是二级指针。
		CommandLineArgumentListType ArgumentList{nullptr};

		/// @brief 默认构造，初始化控制台参数为 nullptr。
		CommandLineArguments();

		/// @brief 构造，初始化控制台参数为指定的值。
		///	@param count 控制台参数的数量。
		///	@param list 控制台参数列表（二级指针）。
		CommandLineArguments(int count, CommandLineArgumentListType list);

		/// @brief 构造，初始化控制台参数为指定的值。
		///	@param count 控制台参数的数量。
		///	@param list 控制台参数列表（二级指针）。
		CommandLineArguments(unsigned int count, CommandLineArgumentListType list);

		/// @brief 访问控制台参数。
		///	如果此时访问的索引超出了范围，那么将会返回空指针。
		CommandLineArgumentType operator[](unsigned int index) const noexcept;
	};
}
