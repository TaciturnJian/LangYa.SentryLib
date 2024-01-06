#include <LangYa/Common/Counter.hpp>

namespace LangYa::Common
{
	Counter
	::Counter()
	= default;

	Counter
	::Counter(const std::uint64_t target) :
		Target(target)
	{
	}

	std::uint64_t
	Counter
	::GetCount() const
	{
		return Number;
	}

	void Counter
	::Reset(const std::uint64_t target)
	{
		Target = target;
		Reset();
	}

	void
	Counter
	::Reset()
	{
		Number = 0;
	}

	bool Counter
	::Count()
	{
		if (Target == 0 || Number >= Target) return true;
		++Number;
		return Number >= Target;
	}
}
