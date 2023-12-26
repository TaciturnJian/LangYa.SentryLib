#pragma once

#ifdef INCLUDE_TEAM_ARMOR_COLOR_H
#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
#error !! ArmorColor.h has been included in c++ mode, and is being included repeatedly !!
#else
#error !! ArmorColor.h has been included in c mode, and is being included repeatedly !!
#endif
#endif

#ifdef __cplusplus
#define INCLUDE_TEAM_ARMOR_COLOR_H 1
#define INCLUDE_TEAM_ARMOR_COLOR_HPP 1
#endif

#ifdef _STDC_
#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
#error !! ArmorColor.h has been included in c++ mode, and is being included repeatedly in c mode !!
#endif

#define INCLUDE_TEAM_ARMOR_COLOR_H 1
#endif

#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
namespace LangYa::SentryLib
{
#endif /* ^^^ 用命名空间包括这个内容 ^^^ */

	enum
#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
	class
#endif /* ^^^ 用 class 使这个enun的内容不至于恶心到整个空间 ^^^ */
	/// @brief 机器人团队枚举类型。
	///	@details 使用 unsigned char 代表机器人的团队，使用位运算表示多种团队的组合。
	RoboMasterTeam
#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
		: unsigned char
#endif
	{
		/// @brief 未知的团队，用于初始化或者发送错误信息。
		RoboMasterTeamUnknown = 0b00000000,

		/// @brief 红队。
		RoboMasterTeamRed     = 0b00000001,

		/// @brief 蓝队。
		RoboMasterTeamBlue    = 0b00000010,

		/// @brief 其他中立或有效目标。
		RoboMasterTeamExtra   = 0b00000100

	}; /* ^^^ 指定为无符号整型，方便 C++ 直接使用 ^^^ */

	enum
#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
	class
#endif /* ^^^ 用 class 使这个enun的内容不至于恶心到整个空间 ^^^ */
	/// @brief 装甲板颜色枚举类型。
	///	@details 使用 unsigned char 代表装甲板的颜色，使用位运算表示多种颜色的组合。
	/// 例如如果你想确认装甲板是否为某个团队的颜色，可以直接使用 & 与 << 移位运算符。
	///	@code
	///	ArmorColor color;
	///	int team = 2; // 假设我们获取到的团队代码是 0b10 也就是蓝色。
	///	if (color & (0b1 << team))	// 判断颜色中是否有蓝色
	///	{
	///		// do something...
	///	}
	///	@endcode
	///	@warning 不要使用用除了蓝色和红色以外的团队来进行位运算，此类型没有设计出这么舒服的操作。
	ArmorColor
#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
		: unsigned char
#endif /* ^^^ 指定为无符号整型，方便 C++ 直接使用 ^^^ */
	{
		/// @brief 未知的颜色，用于初始化或者发送错误信息。
		ArmorColorUnknown = 0b00000000,

		/// @brief 灰色，代表无效目标，但并非逻辑错误。
		ArmorColorGray    = 0b00000001,

		/// @brief 红色。
		ArmorColorRed     = 0b00000010,

		/// @brief 蓝色。
		ArmorColorBlue    = 0b00000100
	};

#ifdef INCLUDE_TEAM_ARMOR_COLOR_HPP
}
#endif /* ^^^ 用命名空间包括这个内容 ^^^ */
