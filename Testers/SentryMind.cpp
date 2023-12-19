#include <LangYa/SentryLib.hpp>

auto Target = {""};

struct Movement
{
	LangYa::SentryLib::Vector2F Velocity{};
	LangYa::SentryLib::Vector2F YawPitch{};
};

void SetMovement(const Movement& movement)
{
	//TODO 实现移动
}

/// 寻找战略点
int GetStrategicPoint()
{
	return 0;
}

int FindPathToPoint()
{
	
}

void FindTarget()
{
	
}

void SearchTarget()
{
	
}

void StandardMind()
{
	while (true)
	{
		// 找到战略点：
		// 获取去战略点的路径

		// 并行执行：
		// 搜索目标
		// 沿着路径移动

		// 中断执行：
		// 发现目标并攻击
	}
}

int main() 
{
	// TODO 加载资源和设备


	// TODO 卸载资源和设备
}