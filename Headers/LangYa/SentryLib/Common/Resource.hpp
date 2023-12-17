#pragma once

#include <atomic>

#include <LangYa/SentryLib/Common/ResourceFlag.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a resource with flag (usage indicators), usually this resource is used in multi-threads.
	///	If you want to get the content, use '=' operator to copy the memory.
	///	The memory may be reading/writing in another thread!
	/// @tparam TContent The type of content, must be trivially copyable.
	template <typename TContent>
	struct Resource final
	{
		/// @brief Indicate the status of the resource.
		std::atomic<ResourceFlag> Flag{ResourceFlag::Empty};

		/// @brief The volatile content. May be reading/writing in different threads.
		TContent Content{};

		/// @brief Default constructor.
		Resource();

		/// @brief Construct with given things.
		///	@param flag Indicate the primary status of the content.
		///	@param content Indicate the primary content.
		Resource(const ResourceFlag& flag, const TContent& content);
	};

#pragma region 函数实现

	template <typename TContent>
	Resource<TContent>::Resource() = default;

	template <typename TContent>
	Resource<TContent>::Resource(const ResourceFlag& flag, const TContent& content): Flag(flag), Content(content)
	{
	}

#pragma endregion
}
