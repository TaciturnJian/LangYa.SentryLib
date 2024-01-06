#pragma once

namespace LangYa::Common
{
	/// @brief 比赛时的所有团队。
	enum class UnitTeam : char
	{
		Red     = -1,
		Neutral = 0,
		Blue    = 1
	};

	/// @brief 比赛时的所有机器人的ID。
	enum class UnitID : char
	{
		Empty     = 0,
		Hero      = 1,
		Engineer  = 2,
		Infantry1 = 3,
		Infantry2 = 4,
		Infantry3 = 5,
		Sentry    = 6,
		Outpost   = 7,
		Buff      = 8,
		Base      = 9
	};

	/// @brief 比赛时的单位种类的编号，可以使用位运算快速操作此类型。
	enum class UnitType : unsigned short
	{
		Hero      = 0b00000001,
		Engineer  = 0b00000010,
		Infantry  = 0b00000100,
		Sentry    = 0b00001000,
		Outpost   = 0b00010000,
		Buff      = 0b00100000,
		Base      = 0b01000000,
		IsBalance = 0b10000000
	};
}
