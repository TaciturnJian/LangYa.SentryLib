#pragma once

namespace LangYa::Common {

	/// @brief 代表控制器，即处理当前对控制器的所有操作，存储到数据中，在合适的时机发送数据给远端。
	/// 如果使用多线程处理，请在传入的模板参数使用原子类型。
	/// @tparam TControllerData 
	template <typename TControllerData>
	class IController {
	public:
		virtual ~IController() = default;
		virtual TControllerData& GetControllerData() = 0;
		virtual const TControllerData& GetControllerData() const = 0;
		virtual bool SendData() = 0;
		TControllerData* operator->()
		{
			return GetControllerData();
		}
		const  TControllerData* operator->() const 
		{
			return GetControllerData();
		}
	};
}