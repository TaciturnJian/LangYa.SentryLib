#pragma once

#include <atomic>

#include <SangoC/Memory/Resources/ResourceFlag.hpp>

namespace LangYa::Sentry
{
	template <typename TContent>
	struct Resource
	{
		std::atomic<ResourceFlag> Flag{ResourceFlag::Empty};

		TContent Content{};

		inline void Lock()
		{
			Flag = ResourceFlag::Busy;
		}

		inline void Unlock(const ResourceFlag flag = ResourceFlag::Empty)
		{
			Flag = flag;
		}

		Resource() = default;

		Resource(const ResourceFlag& flag, const TContent& content) : Flag(flag), Content(content)
		{
		}

		Resource(const Resource& other) = default;

		Resource(Resource&& other) = default;

		Resource& operator=(const Resource& other) = default;

		Resource& operator=(Resource&& other) = default;
	};
}
