#include <LangYa/SentryLib/SentryData.hpp>

LangYa::SentryLib::SentryData::ComplexFireByte
::ComplexFireByte()
= default;

void
LangYa::SentryLib::SentryData::SerializationResult
::operator()(const SentryData& data)
{
	VelocityX = data.Velocity[0];
	VelocityY = data.Velocity[1];
	Yaw = data.GimbalEulerAngle[0];
	Pitch = static_cast<short>(data.GimbalEulerAngle[1] * 100);
	Complex.FireFlag = data.FireFlag;
	Complex.FireFrequency = 0;
	Complex.HoldFlag = 0;
}

void
LangYa::SentryLib::SentryData::DeserializeResource
::operator()(SentryData& data) const
{
	data.GimbalEulerAngle = {Yaw, static_cast<float>(Pitch) / 100.0f};
	data.Velocity = {VelocityX, VelocityY};
	data.AmmoCount = AmmoCount;
	data.FireFlag = FireFlag;
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::SentryData
::GetDeserializationResourceSize() const
{
	return sizeof(DeserializeResource);
}

bool
LangYa::SentryLib::SentryData
::Deserialize(const MemoryView& buffer)
{
	if (buffer.Size < sizeof(DeserializeResource))
	{
		return false;
	}

	const auto& data = *(reinterpret_cast<DeserializeResource*>(buffer.Head));
	data(*this);
	return true;
}

LangYa::SentryLib::MemoryView::SizeType
LangYa::SentryLib::SentryData
::GetSerializationResultSize() const
{
	return sizeof(SerializationResult);
}

bool
LangYa::SentryLib::SentryData
::Serialize(const MemoryView& buffer)
{
	if (buffer.Size < sizeof(SerializationResult))
	{
		return false;
	}

	auto& data = *(reinterpret_cast<SerializationResult*>(buffer.Head));
	data(*this);
	return true;
}
