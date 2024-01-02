#pragma once

#include <ostream>
#include <string>

namespace Sango {
	class IStreamFormat {
	public:
		virtual ~IStreamFormat();
		virtual std::ostream& FormatByStream(std::ostream& stream, int option = 0) const = 0;
	};

	std::string FormatToString(const IStreamFormat& obj, int option = 0);
}
