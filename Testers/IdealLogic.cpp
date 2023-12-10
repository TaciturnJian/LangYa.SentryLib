#include <iostream>

#include <LangYa/SentryLib.hpp>

SC_INTERFACE
VisionModule
{
public:
	bool GetRecognizedArmor(std::vector<LangYa::SentryLib::ArmorPlateInfo>& armors);

};

struct RobotPost
{
	
};

SC_INTERFACE
SLAMModule {
public:

};

void InMatchCallback()
{
	
}

SC_ENTRY_POINT
{
	LangYa::SentryLib::MatchStatusManager manager{};
	manager.LoopRun();
}
