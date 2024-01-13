#pragma once

#define HAS_TEMPLATE_GENERATOR(xMemberName) \
	template<typename T>					\
	struct Has##xMemberName {				\
		using xMemberName##Type = T;		\
		T xMemberName;						\
		T& Get##xMemberName() {return xMemberName;};				\
		const T& Get##xMemberName() const {return xMemberName;};	\
	};										\
	// Generate Has Member

namespace Sango::CommonMember {
	HAS_TEMPLATE_GENERATOR(Name);

	HAS_TEMPLATE_GENERATOR(ID);

	HAS_TEMPLATE_GENERATOR(Info);
}
