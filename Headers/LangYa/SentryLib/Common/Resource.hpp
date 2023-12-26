#pragma once

#include <atomic>

#include <LangYa/SentryLib/Common/ResourceFlag.hpp>

namespace LangYa::SentryLib
{
	/// @brief 代表一个在多线程中流通的资源。
	/// 使用内部的 Flag 为资源上锁。
	/// @tparam TContent 内容的类型，必须是可以使用等号操作符复制的类型。
	template <typename TContent>
	struct Resource final
	{
		/// @brief 代表资源的状态。
		std::atomic<ResourceFlag> Flag{ResourceFlag::Empty};

		/// @brief 可能在不同线程中读取的内容。
		///	如果你需要保证不会出现坏字节，使用等号赋值来取出内部的内容。
		TContent Content{};
		
		/// @brief 默认构造。
		Resource() = default;

		/// @brief 使用指定的内容构造资源。
		///	@param flag 指代资源的初始状态。
		///	@param content 指代初始资源。
		Resource(const ResourceFlag& flag, const TContent& content) : Flag(flag), Content(content)
		{
		}

		/// @brief 使用指针的方式获取内容的内容。
		/// 如果内容是一个结构体，那么利用到 C++ 的特性，你可以通过此指针直接访问它的成员。
		///	@return Content 的指针。
		TContent* operator->()
		{
			return &Content;
		}

		/// @brief 使用指针的方式获取内容的内容。
		///	如果内容是一个结构体，那么利用到 C++ 的特性，你可以通过此指针直接访问它的成员。
		///	@return Content 的常量指针。
		const TContent* operator->() const
		{
			return &Content;
		}
	};
}
