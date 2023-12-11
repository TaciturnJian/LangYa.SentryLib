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
	// 获取决策信息

	// 局部位置决策
	// 如果局部位置决策失败
	// 采用全局位置决策

	// 视角决策

	// 开火决策

	// GetLocalPosition();

	// GetGlobalPosition();

	// 应用决策信息

	// 应用视角信息
	// 应用速度信息
	// 应用开火信息
 }

SC_ENTRY_POINT
{
	LangYa::SentryLib::MatchStatusManager manager{};
	manager.LoopRun();
}
