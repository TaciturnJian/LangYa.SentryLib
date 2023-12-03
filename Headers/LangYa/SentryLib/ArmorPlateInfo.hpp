#pragma once

#include <LangYa/SentryLib/Position3.hpp>
#include <LangYa/SentryLib/Vector3.hpp>

namespace LangYa::SentryLib {
	struct ArmorPlateInfo {
		Position3F Position{};
		Vector3F Normal{};
	};
}