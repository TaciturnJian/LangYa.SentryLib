#pragma once

#define SANGO_FULL_DEFAULT_CLASS(className)					\
	class className {										\
	public:													\
		className() = default;								\
		~className() = default;								\
		className(const className&) = default;				\
		className(className&&) = default;					\
		className& operator=(const className&) = default;	\
		className& operator=(className&&) = default;		\
	};														\
	// Code Snippets For Full Default Class