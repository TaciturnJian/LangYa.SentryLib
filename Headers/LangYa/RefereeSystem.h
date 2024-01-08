#pragma once


#define LANGYA_PACKAGE_HEAD_TYPE unsigned char
#define LANGYA_PACKAGE_HEAD '!'

#ifdef __cplusplus

namespace LangYa
{
	enum class LangYaTeamOreStorageStatusID : unsigned char
	{
		Closed = 0,
		Open   = 1
	};

	enum class LangYaTeamID : unsigned char
	{
		Unknown = 0,
		Red     = 1,
		Blue    = 2
	};

	enum class LangYaLocationID : unsigned char
	{
		Unknown           = 0,
		LargeResourceLand = 1,
		SmallResourceLand = 2,

		RedStart      = 4,
		RedOreStorage = 5,
		RedBuff       = 6,

		BlueStart      = 8,
		BlueOreStorage = 9,
		BlueBuff       = 7
	};

	enum class TaskID : unsigned char
	{
		Unknown = 0,
		Moving  = 1,
		Mining  = 2,
		Aiming  = 3
	};

	enum class OreRewardID : unsigned char
	{
		Unknown = 0,
		Low     = 1,
		Medium  = 2,
		High    = 3
	};
}

#endif

#pragma pack(push, 1)

struct LANGYA_RefereeServerData
{
	unsigned char TeamOreStorageStatusID;
};

struct LANGYA_RefereeClientData
{
	unsigned char TeamID;
	unsigned char LocationID;
	unsigned char TaskID;
	unsigned char OreRewardID;
};

#pragma pack(pop)
