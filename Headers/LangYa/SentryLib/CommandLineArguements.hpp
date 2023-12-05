#pragma once

namespace LangYa::SentryLib {
	using CommandLineArgument = char * const;

	struct CommandLineArguments {
		int ArgumentCount;
		const CommandLineArgument * const ArgumentValues;
	};
}