#pragma once

#include <LangYa/SentryLib/Vector.hpp>
#include <LangYa/SentryLib/Position.hpp>

namespace LangYa::SentryLib
{
	/// @brief Navigator 导航模块
	///	@details Navigator 是自动机器人中的感知器和决策器的重要部分，它提供了如下功能
	///
	///	- 感知自身位置，给出一个三维 float 坐标表示当前位置。
	///	- 感知周围障碍物，给出一系列瞬时二维 float 坐标表示障碍物的位置。
	///	- 给定目标点，给出从当前状态来看的最佳瞬时速度
	///
	/// @warning Navigator 不提供任何运动控制功能，它只是一个决策器，它的输出需要由运动控制器来执行。
	///
	// ReSharper disable once CppClassCanBeFinal
	class Navigator
	{
	public:
		/// @brief C++的接口都需要这个析构函数。
		///	@warning 别给我删了！(╯▔皿▔)╯
		virtual ~Navigator();

		/// @brief DestinationIDType 目标点的 ID 类型。
		///	@details 会有一个 Manager 提供 ID 和具体目标地点的相互映射，因此仅从 ID 就能查询到目标坐标。
		using DestinationIDType = int;

		/// @brief 获取自身位置。
		///	@details 当自身位置更新时，该函数会将自身位置写入到 selfLocation 变量中。
		///	如果没有更新，则获取自身位置失败，返回 false 。
		///	@param selfLocation 用于接收自身位置的变量。
		///	@return 是否成功获取到自身位置。
		virtual bool GetSelfLocation(Position3F& selfLocation);

		/// @brief 给定目标地点，获取到达目标地点的速度。
		///	@details 使用此函数将设置导航器的目的地为 destination 。
		///	之后，导航器会做一系列计算，并返回到达目的地的速度。
		///	@warning 返回 false 并不能完全代表导航器解算速度失败，可能是解算太慢等原因。
		///	如果需要判断此函数当前不可用，请在一段时间内连续调用此函数。
		///	连续返回 false 才说明导航器真正寄了。
		///	@param destination 传入目的地的 ID 。
		///	@param velocity 用于接收速度的变量。
		///	@return 如果解算出来了速度，则返回 true ，否则返回 false。
		virtual bool GetVelocityToDestination(const DestinationIDType& destination, Vector2F& velocity);

		/// @brief 获取周围的障碍物位置。
		///	@details 每次调用此函数都会将新的障碍物位置写入到 obstacles 变量中。
		///	@warning 此函数不负责清空 vector，如果需要，请手动调用 clear 。
		///	@param obstacles 用于接收障碍物位置的 vector 。
		///	@return 是否成功获取到任何障碍物位置。
		virtual bool GetObstaclePosition(std::vector<Position2F>& obstacles);
	};
}
