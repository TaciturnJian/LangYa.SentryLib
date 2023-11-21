#pragma once

#include <SangoC/Memory/Serializer.hpp>
#include <SangoC/Robotic/Sensor.hpp>
#include <SangoC/Robotic/Sentry/SentrySensorData.hpp>
#include <SangoC/Robotic/Sentry/CompressedSentrySensorData.hpp>

namespace SangoC::Robotic::Sentry 
{
	using SentrySensor = Sensor<SentrySensorData, sizeof(CompressedSentrySensorData)>;
}

template<>
class SangoC::Memory::Serializer<SangoC::Robotic::Sentry::SentrySensorData, sizeof(SangoC::Robotic::Sentry::CompressedSentrySensorData)>
{
public:
	static bool Deserialize(const MemoryView& buffer, Robotic::Sentry::SentrySensorData& resource)
	{
		if (buffer.Size < sizeof(Robotic::Sentry::CompressedSentrySensorData)) return false;

		//TODO Data Verification

		reinterpret_cast<Robotic::Sentry::CompressedSentrySensorData*>(buffer.Head)[0](resource);

		return true;
	}
};
