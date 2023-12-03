#include <LangYa/SentryLib.hpp>

using namespace LangYa::SentryLib;

#pragma region Data Structures

struct EmbeddedSoftwareSensorData final : DeserializableContent
{
#pragma pack(push, 1)
	struct DeserializationSource
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetDeserializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

struct EmbeddedSoftwareControllerData final : SerializableContent
{
#pragma pack(push, 1)
	struct SerializationResult
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetSerializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

struct AutoAimerSoftwareSensorData final : DeserializableContent
{
#pragma pack(push, 1)
	struct DeserializationSource
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetDeserializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

struct AutoAimerSoftwareControllerData final : SerializableContent
{
#pragma pack(push, 1)
	struct SerializationResult
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetSerializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

struct NavigatorSoftwareSensorData final : DeserializableContent
{
#pragma pack(push, 1)
	struct DeserializationSource
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetDeserializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

struct NavigatorSoftwareControllerData final : SerializableContent
{
#pragma pack(push, 1)
	struct SerializationResult
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetSerializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

struct RefereeSystemSensorData final : DeserializableContent
{
#pragma pack(push, 1)
	struct DeserializationSource
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetDeserializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

struct RefereeSystemControllerData final : SerializableContent
{
#pragma pack(push, 1)
	struct SerializationResult
	{
	};
#pragma pack(pop)

	[[nodiscard]] MemoryView GetSerializationMemoryView() const override
	{
		return MemoryView{this, 1};
	}
};

#pragma endregion

#pragma region Connections

std::shared_ptr<Connection> EmbeddedSoftwareRawConnection{nullptr};
std::shared_ptr<Connection> AutoAimerSoftwareRawConnection{nullptr};
std::shared_ptr<Connection> NavigatorSoftwareRawConnection{nullptr};
std::shared_ptr<Connection> RefereeSystemRawConnection{nullptr};

std::shared_ptr<LangYaConnection> EmbeddedSoftwareConnection{nullptr};
std::shared_ptr<LangYaConnection> AutoAimerSoftwareConnection{nullptr};
std::shared_ptr<LangYaConnection> NavigatorSoftwareConnection{nullptr};
std::shared_ptr<LangYaConnection> RefereeSystemConnection{nullptr};

#pragma endregion

#pragma region Devices

std::shared_ptr<Sensor<EmbeddedSoftwareSensorData>> EmbeddedSoftwareSensor{nullptr};
std::shared_ptr<Controller<EmbeddedSoftwareControllerData>> EmbeddedSoftwareController{nullptr};

std::shared_ptr<Sensor<EmbeddedSoftwareSensorData>> AutoAimerSoftwareSensor{nullptr};
std::shared_ptr<Controller<EmbeddedSoftwareControllerData>> AutoAimerSoftwareController{nullptr};

std::shared_ptr<Sensor<EmbeddedSoftwareSensorData>> NavigatorSoftwareSensor{nullptr};
std::shared_ptr<Controller<EmbeddedSoftwareControllerData>> NavigatorSoftwareController{nullptr};

std::shared_ptr<Sensor<EmbeddedSoftwareSensorData>> RefereeSystemSensor{nullptr};
std::shared_ptr<Controller<EmbeddedSoftwareControllerData>> RefereeSystemController{nullptr};

#pragma endregion

struct DecisionResource
{
	
};

struct DecisionResult
{
	
};

void CollectionInformation(DecisionResource& resource, const DecisionResult& lastResult)
{

}

namespace Behavior
{
	void LookAt()
	{
		
	}

	void ShootAt()
	{
		
	}

	void Attack()
	{
		ShootAt();
	}

	void Defense()
	{
		LookAt();
	}

	void Move()
	{
		LookAt();
	}

	void IWantTo()
	{
		Attack();
		Defense();
		Move();
	}
}

void GenerateDecision(const DecisionResource& resource, DecisionResult& result)
{
	Behavior::IWantTo();
}

void ApplyDecision(DecisionResult& result)
{
	
}

int main()
{

	return 0;
}
