#pragma once

#include <string>
#include <functional>

#include <boost/json.hpp>
#include <boost/program_options.hpp>

#include <LangYa/SentryLib/CommandLineArguments.hpp>

namespace LangYa::SentryLib
{
	/// @brief 程序配置文件和配置参数处理器。
	///	提供了对命令行参数和json配置文件的处理工具，使用操作见 @file ../Testers/TestApp.cpp
	class Configurator
	{
	public:
		/// @brief boost中储存命令行参数的容器。
		using CommandLineArgumentContainer = boost::program_options::options_description;

		/// @brief boost中储存变量的容器。
		using CommandLineVariableContainer = boost::program_options::variables_map;

		/// @brief 将json数据映射到内存的函数
		///	其参数为 json 表的引用，返回值为是否成功。
		using JsonMapper = std::function<bool(boost::json::value&)>;

	private:
		/// @brief boost中储存命令行参数的容器。
		CommandLineArgumentContainer OptionsWithDescription{};

		/// @brief boost中储存变量的容器。
		CommandLineVariableContainer VariablesMap{};

	public:
		/// @brief 初始化一个空的配置器
		Configurator();

		/// @brief 禁止拷贝构造。
		Configurator(const Configurator&) = delete;

		/// @brief 禁止移动构造。
		Configurator(Configurator&&) = delete;

		boost::program_options::options_description_easy_init AddOption();

		[[nodiscard]] bool operator()(const std::string& flag) const;

		bool Load(const CommandLineArguments& commandLineArguments);

		[[nodiscard]] std::string GetHelpContent() const;

		static bool Load(const std::string& jsonFilePath, const JsonMapper& mapper);
	};
}
