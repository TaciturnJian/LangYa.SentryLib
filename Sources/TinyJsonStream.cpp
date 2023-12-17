#include <LangYa/SentryLib/Common/TinyJsonStream.hpp>

LangYa::SentryLib::TinyJsonStream
::TinyJsonStream(PackageCallbackType callback):
	Callback(std::move(callback))
{
}

LangYa::SentryLib::TinyJsonStream&
LangYa::SentryLib::TinyJsonStream
::operator<<(const char ch)
{
	bool received_package{false};
	switch (ch)
	{
	case '{':
		if (JsonLevel == 0)
			InJson = true;

		JsonLevel++;
		break;

	case '}':
		if (JsonLevel == 1)
			received_package = true;

		if (JsonLevel == 0)
			//TODO 处理异常
			break;

		JsonLevel--;
		break;

	default:
		break;
	}

	if (InJson)
	{
		Stream << ch;
	}

	if (received_package)
	{
		InJson = false;
		Callback(Stream.str());
		Stream.str("");
	}

	return *this;
}
