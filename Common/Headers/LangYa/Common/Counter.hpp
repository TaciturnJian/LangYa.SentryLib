#pragma once

#include <cstdint>

namespace LangYa::Common
{
	/// @brief 计数器，在到达目标计数时触发。
	class Counter final
	{
		std::uint64_t Number{0};
		std::uint64_t Target{0};

	public:
		Counter();

		explicit Counter(std::uint64_t target);

		[[nodiscard]] std::uint64_t GetCount() const;

		void Reset(std::uint64_t target);

		void Reset();

		bool Count();
	};
}
