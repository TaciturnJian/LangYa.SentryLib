#pragma once

#include <functional>
#include <string>
#include <sstream>

namespace LangYa::SentryLib
{
	class TinyJsonStream
	{
	public:
		using PackageCallbackType = std::function<void(const std::string&)>;

	private:
		unsigned int JsonLevel{0};
		std::stringstream Stream{};
		bool InJson{false};
		PackageCallbackType Callback;

	public:
		explicit TinyJsonStream(PackageCallbackType callback);

		TinyJsonStream& operator<<(char ch);
	};
}
