#pragma once

#include <LangYa/SentryLib/Common/DeserializableContent.hpp>
#include <LangYa/SentryLib/Common/SerializableContent.hpp>
#include <LangYa/SentryLib/Math/Vector2.hpp>

namespace LangYa::SentryLib
{
	struct SentryData final : SerializableContent, DeserializableContent
	{
		/// @brief 云台欧拉角，单位是度。
		Vector2F GimbalEulerAngle{0, 0};

		/// @brief 速度向量，元素的范围是 [-100, 100] 每个值代表档位。
		Vector2C Velocity{0, 0};

		/// @brief 子弹数量。
		unsigned short AmmoCount{0};

		/// @brief 开火标志位，当取反时表示开火。
		bool FireFlag{false};

#pragma region 便于序列化而创建的一些结构体

#pragma pack(push,1)

		struct ComplexFireByte
		{
			unsigned char FireFlag      : 2 = 0;
			unsigned char HoldFlag      : 2 = 0;
			unsigned char FireFrequency : 4 = 0;

			ComplexFireByte();
		};

		struct SerializationResult final
		{
			char VelocityX{0};         // Integers in [-100, 100]
			char VelocityY{0};         // Integers in [-100, 100]
			float Yaw{0};              // 1 = 1 Degree
			short Pitch{0};            // 1 = 0.01 Degree
			ComplexFireByte Complex{}; // Complex byte for fire
			unsigned char CRCCode{0};  // CRC8

			void operator()(const SentryData& data);
		};

		struct DeserializeResource final
		{
			char Head{'!'};
			float Yaw{0};                // 1 = 1°
			short Pitch{0};              // 1 = 0.01°
			unsigned short AmmoCount{0}; // 1 = 一发子弹
			char VelocityX{0};           // [-100, 100]的整数
			char VelocityY{0};           // [-100, 100]的整数
			bool FireFlag{false};        // 当前开火标志（取反后开火）
			unsigned char CRCCode{0};    // CRC8验证位置

			void operator()(SentryData& data) const;
		};

#pragma pack(pop)

#pragma endregion

		[[nodiscard]] MemoryView::SizeType GetDeserializationResourceSize() const override;

		[[nodiscard]] bool Deserialize(const MemoryView& buffer) override;

		[[nodiscard]] MemoryView::SizeType GetSerializationResultSize() const override;

		[[nodiscard]] bool Serialize(const MemoryView& buffer) override;
	};
}
