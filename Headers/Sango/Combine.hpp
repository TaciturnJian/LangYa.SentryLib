#pragma once

namespace Sango {
	template<typename ...TList>
	struct Combine;

	template<>
	struct Combine<>{};

	template<typename TFirst, typename ...TRest>
	struct Combine<TFirst, TRest...> : TFirst, Combine<TRest...>
	{
	};
}