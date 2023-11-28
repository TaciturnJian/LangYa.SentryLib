#pragma once

#include <LangYa/SentryLib/MemoryView.hpp>

namespace LangYa::SentryLib {
	/// @brief Represent a io that read/write with @code LangYa::SentryLib::MemoryView @endcode.
	class MemoryIO {
	public:
		/// @brief Virtual destructor.
		virtual ~MemoryIO();

		/// @brief Read bytes from the io.
		///	@param view The view to put the bytes read from the io.
		///	@return The number of bytes read from the io.
		virtual MemoryView::SizeType Read(const MemoryView& view) = 0;

		/// @brief Write bytes to the io.
		///	@param view The view where the bytes to be written are.
		///	@return The number of bytes written to the io.
		virtual MemoryView::SizeType Write(const MemoryView& view) = 0;
	};
}