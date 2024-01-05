#pragma once

#include <memory>
#include <cstdint>

namespace LangYa::SentryLib
{
	/// @brief 动态堆栈，将元素储存在堆栈上，保证不会发生内存泄漏。
	/// @details
	///	# DynamicStack 动态堆栈
	///
	///	动态堆栈代表一个内存空间，帮助大量使用共享指针的类管理内存。
	///	交给动态堆栈的指针为共享指针，会在所有此共享指针被销毁时释放指向的内存。
	///
	///	## 动态堆栈的使用方法
	///
	///	```C++
	///	DynamicStack<T> stack{};
	///	auto shared_element = std::make_shared<T>();
	///	auto& space = stack.RestoreSpace();
	///	space = shared_element;
	///	```
	///	@author Sango
	template <typename TElement>
	class DynamicStack final
	{
	public:
		/// @brief 元素指针，指向的元素会在最后一个共享指针被销毁时释放。
		using ElementPointer = std::shared_ptr<TElement>;

	private:
		/// @brief 堆栈的一个槽位。
		/// @details 非常简单的一个单向链表节点。
		struct StackSlot
		{
			/// @brief 槽位存储的元素，空指针表示当前资源为空，可被替换为的新的元素。
			///	@details 通过共享指针与外界分享的元素。
			ElementPointer Element{nullptr};

			/// @brief 指向下一个槽位的元素。
			std::unique_ptr<StackSlot> Next{nullptr};
		};

		/// @brief 所有元素的第一个槽位。
		std::unique_ptr<StackSlot> FirstSlot{std::make_unique<StackSlot>()};

	public:
		/// @brief 获取可用的元素，如果没有就创建一个。
		///	@details 请确保获得元素的指针后，处理完成后再获取下一个元素。即在调用此函数后必须保证 ElementPointer 不再为空指针。
		///	当你需要把元素传递给另一个无法精确控制何时关闭的线程时，复制一份此指针，保证在此堆栈销毁后指针指向的内存不被一起销毁。
		///	指向的内存会在最后一个共享指针被销毁时释放。
		ElementPointer& RestoreSpace()
		{
			using CounterType = std::uint64_t;
			constexpr CounterType max_count = 0xffffffff;

			std::unique_ptr<StackSlot>* pointer_to_current_slot_pointer = &FirstSlot;
			for (CounterType index{}; index < max_count; ++index)
			{
				auto& slot_pointer = *pointer_to_current_slot_pointer;
				auto& slot = *slot_pointer;
				auto& element_pointer = slot.Element;
				if (element_pointer == nullptr) return element_pointer;

				auto& next_slot_pointer = slot.Next;
				if (next_slot_pointer == nullptr)
				{
					next_slot_pointer = std::move(std::make_unique<StackSlot>());
					return next_slot_pointer->Element;
				}

				pointer_to_current_slot_pointer = &slot.Next;
			}

			throw std::exception(); // TODO 指定消息，太多元素，装不下了。
		}
	};
}
