#include <sstream>

#include <Sango/IStreamFormat.hpp>

namespace Sango
{
	IStreamFormat::~IStreamFormat() = default;

	std::string FormatToString(const IStreamFormat& obj, const int option)
	{
		std::stringstream stream{};
		obj.FormatByStream(stream, option);
		return stream.str();
	}
}
