#pragma once

#include <SangoC/Memory/Serializer.hpp>
#include <SangoC/Robotic/Controller.hpp>
#include <SangoC/Robotic/Sentry/SentryControllerData.hpp>
#include <SangoC/Robotic/Sentry/CompressedSentryControllerData.hpp>


namespace SangoC::Robotic::Sentry
{

	using SentryController = Controller<SentryControlData, sizeof(CompressedSentryControllerData)>;
}

template <>
class SangoC::Memory::Serializer
	<
		SangoC::Robotic::Sentry::SentryControlData,
		sizeof(SangoC::Robotic::Sentry::CompressedSentryControllerData)
	>
{
public:
	static bool Serialize(const Robotic::Sentry::SentryControlData& resource, const MemoryView& buffer)
	{
		if (buffer.Size < sizeof(Robotic::Sentry::CompressedSentryControllerData)) return false;

		auto& compressed = *reinterpret_cast<Robotic::Sentry::CompressedSentryControllerData*>(buffer.Head);
		compressed = resource;

		//TODO CRC8 Encode

		return true;
	}
};
