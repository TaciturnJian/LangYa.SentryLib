#pragma once

#include <boost/asio/buffer.hpp>

namespace SangoC::Memory
{
	struct MemoryView
	{
		using SizeType = unsigned long long;
		using ByteType = unsigned char;
		using HeadType = ByteType*;

		HeadType Head{nullptr};
		SizeType Size{0};

		MemoryView();

		template <typename THead>
		MemoryView(THead* const head, const SizeType& count) :
			// ReSharper disable once CppCStyleCast
			Head((ByteType*)head),
			Size(count * sizeof(THead))
		{
		}

		MemoryView(const MemoryView& view, const SizeType& offset, const SizeType& size = 0);

		MemoryView(const MemoryView& other);

		MemoryView(MemoryView&& other) noexcept;

		MemoryView& operator=(const MemoryView& other);

		MemoryView& operator=(MemoryView&& other) noexcept;

		ByteType& operator[](const SizeType& index) const;

		SizeType ReadFrom(const void* head) const;

		SizeType ReadFrom(const MemoryView& view) const;  // NOLINT(modernize-use-nodiscard)

		SizeType CopyTo(void* head) const;

		SizeType CopyTo(const MemoryView& view) const;  // NOLINT(modernize-use-nodiscard)

		~MemoryView();

		[[nodiscard]] bool IsValid() const noexcept;

		[[nodiscard]] boost::asio::mutable_buffer ToBuffer() const;
	};
}
