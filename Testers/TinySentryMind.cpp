#include <LangYa/SentryLib.hpp>

using namespace LangYa::SentryLib;

bool GetIsMatchStarted()
{
	return false;
}

int FindDestinationID()
{
	return 0;
}

void SetDestination(int destination)
{
}

int SearchForEnemy()
{
	return 0;	
}

void SetEnemy(int enemy)
{
}

void BeforeMatch(std::atomic_bool& isMatchBegin)
{
	// 比赛开始，提前退出
	if (GetIsMatchStarted())
	{
		isMatchBegin = true;
		return;
	}

	isMatchBegin = false;

	// TODO 调试行为
}

void InMatch(std::atomic_bool& isMatchEnd)
{
	// 比赛结束，提前退出
	if (GetIsMatchStarted())
	{
		isMatchEnd = true;
		return;
	}

	isMatchEnd = false;

	// TODO 比赛中行为

	// 获取最佳目的地
	const auto destination = FindDestinationID();
	SetDestination(destination);

	// 寻找敌人
	const auto enemy = SearchForEnemy();
	// 看到敌人就打
	SetEnemy(enemy);
}

void AfterMatch(std::atomic_bool& isMatchReset)
{
	// 比赛重置，提前退出
	if (GetIsMatchStarted())
	{
		isMatchReset = true;
		return;
	}

	isMatchReset = false;

	// TODO 比赛结束行为
}

int main() 
{
	MatchStatusManager manager{BeforeMatch, InMatch, AfterMatch};

	return 0;
}
