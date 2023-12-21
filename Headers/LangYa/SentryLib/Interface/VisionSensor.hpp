#pragma once
#include <vector>

#include <LangYa/SentryLib/Interface/ArmorPlate.hpp>
#include <LangYa/SentryLib/Interface/ExchangeCube.hpp>

namespace LangYa::SentryLib {
	/// @brief VisonSensor 视觉传感器，提供机器人视觉相关信息。
	///	@details VisonSensor 为机器人提供了额外的信息源，目前它能获取如下信息：
	///
	///	- 提供视觉传感器上捕获到的装甲板的原始信息
	///	- 因为工程需要，添加了额外的一个接口，用于获取兑换框信息。
	///
	// ReSharper disable once CppClassCanBeFinal
	class VisionSensor
	{
	public:
		virtual ~VisionSensor();

		/// @brief 获取最新识别的装甲板信息列表。
		///	@param armors 装甲板信息列表。
		virtual bool GetArmorPlates(std::vector<ArmorPlate>& armors);

		/// @brief 获取兑换框的信息。
		///	@param cube 兑换框。
		virtual bool GetExchangeCube(ExchangeCube& cube);
	};

}
