#pragma once

#include <atomic>

#include <LangYa/SentryLib/ResourceFlag.hpp>

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

		/// @brief Default copy constructor.
		///	@param other The other resource.
		Resource(const Resource& other);

		/// @brief Default move constructor.
		///	@param other The other resource.
		Resource(Resource&& other) noexcept;

		/// @brief Default copy assignment operator.
		///	@param other The other resource.
		///	@return The reference to this.
		Resource& operator=(const Resource& other);

		/// @brief Default move assignment operator.
		///	@param other The other resource.
		///	@return The reference to this.
		Resource& operator=(Resource&& other) noexcept;
	};

#pragma region 函数实现

	template <typename TContent>
	Resource<TContent>::Resource() = default;

	template <typename TContent>
	Resource<TContent>::Resource(const ResourceFlag& flag, const TContent& content): Flag(flag), Content(content)
	{
	}

	template <typename TContent>
	Resource<TContent>::Resource(const Resource& other) = default;
	template <typename TContent>
	Resource<TContent>::Resource(Resource&& other) noexcept = default;
	template <typename TContent>
	Resource<TContent>& Resource<TContent>::operator=(const Resource& other) = default;
	template <typename TContent>
	Resource<TContent>& Resource<TContent>::operator=(Resource&& other) noexcept = default;

#pragma endregion
}
