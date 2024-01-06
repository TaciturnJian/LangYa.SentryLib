#pragma once

#include <LangYa/SentryLib/Interface/ArmorPlate.hpp>

#include <LangYa/Common/ISerializable.hpp>

namespace LangYa::SentryLib
{
	// 裁判数据
	struct RefereeData final : Common::ISerializable
	{
		enum class MatchStatus : char
		{
			BeforeMatch = 0,
			InMatch     = 1,
			AfterMatch  = 2
		};

		MatchStatus CurrentMatchStatus{MatchStatus::BeforeMatch};
		unsigned OutpostHealth{1000};

		bool IUseRevive{false};
		bool IUseReload{false};

		[[nodiscard]] Common::MemoryView::SizeType GetDeserializationResourceSize() const override
		{
			return sizeof(CurrentMatchStatus) + sizeof(OutpostHealth);
		}

		[[nodiscard]] bool Deserialize(const Common::MemoryView& buffer) override
		{
			if (buffer.Size < GetDeserializationResourceSize())
			{
				return false;
			}

			auto ptr = buffer.Head;
			CurrentMatchStatus = *reinterpret_cast<const MatchStatus*>(ptr);
			ptr += sizeof(CurrentMatchStatus);
			OutpostHealth = *reinterpret_cast<const unsigned*>(ptr);
			return true;
		}

		[[nodiscard]] Common::MemoryView::SizeType GetSerializationResultSize() const override
		{
			return sizeof(CurrentMatchStatus) + sizeof(OutpostHealth);
		}

		[[nodiscard]] bool Serialize(const Common::MemoryView& buffer) override
		{
			if (buffer.Size < GetSerializationResultSize())
			{
				return false;
			}

			auto ptr = buffer.Head;
			*reinterpret_cast<MatchStatus*>(ptr) = CurrentMatchStatus;
			ptr += sizeof(CurrentMatchStatus);
			*reinterpret_cast<unsigned*>(ptr) = OutpostHealth;
			return true;
		}

		SizeType GetSerializationResultSize() override
		{
			
		}
	};

	// 控制器数据
	struct FullControlData final : DeserializableContent, SerializableContent
	{
		int HitTarget{0};
		int MoveDestination{0};
		bool StartScanMode{false};

		[[nodiscard]] Common::MemoryView::SizeType GetDeserializationResourceSize() const override
		{
			return sizeof(HitTarget) + sizeof(MoveDestination) + sizeof(StartScanMode);
		}

		[[nodiscard]] bool Deserialize(const Common::MemoryView& buffer) override
		{
			if (buffer.Size < GetDeserializationResourceSize())
			{
				return false;
			}

			auto ptr = buffer.Head;
			HitTarget = *reinterpret_cast<const int*>(ptr);
			ptr += sizeof(HitTarget);
			MoveDestination = *reinterpret_cast<const int*>(ptr);
			ptr += sizeof(MoveDestination);
			StartScanMode = *reinterpret_cast<const bool*>(ptr);
			return true;
		}

		[[nodiscard]] Common::MemoryView::SizeType GetSerializationResultSize() const override
		{
			return sizeof(HitTarget) + sizeof(MoveDestination) + sizeof(StartScanMode);
		}

		[[nodiscard]] bool Serialize(const Common::MemoryView& buffer) override
		{
			if (buffer.Size < GetSerializationResultSize())
			{
				return false;
			}

			auto ptr = buffer.Head;
			*reinterpret_cast<int*>(ptr) = HitTarget;
			ptr += sizeof(HitTarget);
			*reinterpret_cast<int*>(ptr) = MoveDestination;
			ptr += sizeof(MoveDestination);
			*reinterpret_cast<bool*>(ptr) = StartScanMode;
			return true;
		}
	};

	// 传感器数据
	struct FullSensorData final : DeserializableContent, SerializableContent
	{
		ArmorPlate LatestArmorPlate{};

		[[nodiscard]] Common::MemoryView::SizeType GetDeserializationResourceSize() const override
		{
			return sizeof(LatestArmorPlate);
		}

		[[nodiscard]] bool Deserialize(const Common::MemoryView& buffer) override
		{
			//TODO 未完成
			if (buffer.Size == 0)
			{
				return false;
			}

			if (buffer[0] == 1)
			{
				LatestArmorPlate.Particle.Position = {0, 1, 0};
				LatestArmorPlate.Particle.Direction = {0, 0, 0};
				LatestArmorPlate.ID = 1;
			}
			else
			{
				LatestArmorPlate = ArmorPlate{};
			}
			return true;
		}

		[[nodiscard]] Common::MemoryView::SizeType GetSerializationResultSize() const override
		{
			return sizeof(LatestArmorPlate);
		}

		[[nodiscard]] bool Serialize(const Common::MemoryView& buffer) override
		{
			if (buffer.Size == 0)
			{
				return false;
			}

			buffer[0] = static_cast<Common::MemoryView::ByteType>(LatestArmorPlate.ID);
			return true;
		}
	};
}
