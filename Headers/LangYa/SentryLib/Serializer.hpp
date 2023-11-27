#pragma once

#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib
{
	/// @brief Represent the serializer, encode the data input and decode them into char array.
	///	It's basically the static class in C#, but there is no such thing in c++, so it's a strange implementation.
	///	TODO Write an example for this class
	///	@tparam TResource The type for the resource content.
	///	@tparam ResultSize The size for the compressed data.
	template<typename TResource, MemoryView::SizeType ResultSize>
	class Serializer
	{
	public:
		Serializer() = delete;
		~Serializer() = delete;

		/// @brief Indicate the size of data.
		inline static MemoryView::SizeType CompressedResourceSize{ResultSize};

		/// @brief Template function for serialization.
		///	Every static function must be with the same signature of @code Serializer::Serialize @endcode.
		///	@param resource The copyable resource for encoding.
		///	@param buffer The destination for data encoded.
		static bool Serialize(const TResource& resource, const MemoryView& buffer)
		{
			return false;
		}

		/// @brife Template function for deserialization.
		/// Every static function must be with the same signature of @code Serializer::Deserialize @endcode.
		///	@param resource The mutable resource to store the decoded data.
		///	@param buffer The source where the data requires decode.
		static bool Deserialize(const MemoryView& buffer, TResource& resource)
		{
			return false;
		}
	};

}