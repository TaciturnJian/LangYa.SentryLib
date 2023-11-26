#pragma once

#include <atomic>

#include <LangYa/SentryLib/UniqueBuffer.hpp>
#include <LangYa/SentryLib/ResourceFlag.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent a resource with flag (usage indicators), usually this resource is used in multi-threads.
	///	If you want to get the content, use '=' operator to copy the memory.
	///	The memory may be reading/writing in another thread!
	/// @tparam TContent The type of content, must be trivially copyable.
	template <typename TContent>
	struct Resource
	{
		/// @brief Indicate the status of the resource.
		std::atomic<ResourceFlag> Flag{ResourceFlag::Empty};

		/// @brief The volatile content. May be reading/writing in different threads.
		TContent Content{};

		/// @brief Default constructor.
		Resource() = default;

		/// @brief Construct with given things.
		///	@param flag Indicate the primary status of the content.
		///	@param content Indicate the primary content.
		Resource(const ResourceFlag& flag, const TContent& content) : Flag(flag), Content(content)
		{
		}

		/// @brief Default copy constructor.
		///	@param other The other resource.
		Resource(const Resource& other) = default;

		/// @brief Default move constructor.
		///	@param other The other resource.
		Resource(Resource&& other) = default;

		/// @brief Default copy assignment operator.
		///	@param other The other resource.
		///	@return The reference to this.
		Resource& operator=(const Resource& other) = default;

		/// @brief Default move assignment operator.
		///	@param other The other resource.
		///	@return The reference to this.
		Resource& operator=(Resource&& other) = default;
	};
}
