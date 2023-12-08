#pragma once

namespace LangYa::SentryLib
{
	/// @brief 使用一个元素 X 代表坐标
	///	TODO 注释
	template <typename TElement>
	struct Position1
	{
		TElement X{0};

		using Myself = Position1&;
		Position1() = default;

		explicit Position1(const TElement& x) : X{x}
		{
		}

		~Position1() = default;
	};
}
