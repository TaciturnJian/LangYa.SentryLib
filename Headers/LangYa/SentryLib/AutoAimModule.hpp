#pragma once

#include <opencv2/opencv.hpp>

#include <LangYa/SentryLib/Device.hpp>

namespace LangYa::SentryLib {
	/// @brief 代表一个相机设备
	class CameraDevice : public Device
	{
	public:
		using PictureType = cv::Mat;

		virtual bool GetPicture(PictureType& picture) = 0;
	};

	/// @brief 辅助瞄准模块，提供机器人视觉相关内容
	class AutoAimModule {
	public:
		using PictureType = cv::Mat;



	};
}
