#include <iostream>

#include <LangYa/SentryLib.hpp>

class
	VisionModule
{
public:
	bool GetRecognizedArmor(std::vector<LangYa::SentryLib::ArmorPlateInfo>& armors);
};

SC_ENTRY_POINT
{
	LangYa::SentryLib::MatchStatusManager manager{};
	manager.LoopRun();
}
