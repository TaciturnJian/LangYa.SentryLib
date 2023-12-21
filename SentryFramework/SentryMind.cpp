#include <boost/asio/io_context.hpp>

#include <spdlog/spdlog.h>

#include <LangYa/SentryLib/Application/MonitorPackage.hpp>
#include <LangYa/SentryLib/Common/TCPConnection.hpp>
#include <LangYa/SentryLib/Application/Configurator.hpp>
#include <LangYa/SentryLib/MatchStatusManager.hpp>
#include <LangYa/SentryLib/Application/ApplicationInfo.hpp>
#include <LangYa/SentryLib/DeserializableContentSensor.hpp>
#include <LangYa/SentryLib/SerializableContentController.hpp>
#include <LangYa/SentryLib/TestData.hpp>

LangYa::SentryLib::ApplicationInfo AppInfo{
	"SentryMind",
	"A tiny logic system for match",
	{1, 0, 0, 0}
};

int main()
{
	using namespace LangYa::SentryLib;

	// 初始化日志
	using Configurator = Configurator;
	Configurator configurator{};
	if (!Configurator::InitializeMultiLogger())
	{
		spdlog::error("Failed to initialize MultiLogger");
		return -1;
	}

	// 显示应用程序信息
	AppInfo.OutputTo_spdlog();

	// 打开与显示器的连接
	boost::asio::io_context io_context{};
	const auto monitor_connection = TCPConnection::BuildShared(io_context, "127.0.0.1", 8989);
	monitor_connection->Connect();
	if (!monitor_connection->IsConnected())
	{
		spdlog::error("Failed to connect to monitor");
		return -1;
	}

	// 打开与其他系统的连接
	const auto referee_connection = TCPConnection::BuildShared(io_context, "127.0.0.1", 8990);
	referee_connection->Connect();
	if (!referee_connection->IsConnected())
	{
		spdlog::error("Failed to connect to referee");
		return -1;
	}

	const auto device_connection = TCPConnection::BuildShared(io_context, "127.0.0.1", 8991);
	device_connection->Connect();
	if (!device_connection->IsConnected())
	{
		spdlog::error("Failed to connect to device");
		return -1;
	}

	// 初始化传感器与控制器
	auto shared_full_data_controller = std::make_shared<SerializableContentController<FullControlData>>(
		device_connection);
	auto shared_full_data_sensor = std::make_shared<DeserializableContentSensor<FullSensorData>>(device_connection);
	auto shared_referee_sensor = std::make_shared<DeserializableContentSensor<RefereeData>>(referee_connection);
	auto shared_referee_controller = std::make_shared<SerializableContentController<RefereeData>>(referee_connection);

	std::thread device_ticking{
		[=]
		{
			std::thread controller_tick{
				[=]
				{
					while (true)
					{
						auto& controller = *shared_full_data_controller;
						controller.Tick();
					}
				}
			};
			std::thread referee_controller_tick{
				[=]
				{
					while (true)
					{
						auto& controller = *shared_referee_controller;
						controller.Tick();
					}
				}
			};
			std::thread sensor_tick{
				[=]
				{
					while (true)
					{
						auto& sensor = *shared_full_data_sensor;
						sensor.Tick();
					}
				}
			};
			std::thread referee_sensor_tick{
				[=]
				{
					while (true)
					{
						auto& sensor = *shared_referee_sensor;
						sensor.Tick();
					}
				}
			};

			controller_tick.join();
			referee_controller_tick.join();
			sensor_tick.join();
			referee_sensor_tick.join();
		}
	};
	device_ticking.detach();

	// 初始化监控包
	MonitorPackage mind_status_package{
		"SentryMind",
		{
			{"BeforeMatch", 1},
			{"InMatch", 0},
			{"EndMatch", 0},
			{"FoundTarget", 0},
			{"ExecutingTarget", 0},
			{"Destination", 0}
		}
	};

	// 初始化比赛状态回调器
	MatchStatusManager match_status_manager{
		[&mind_status_package, shared_referee_sensor, monitor_connection](std::atomic_bool& matchBegin)
		{
			mind_status_package.WriteJsonToSharedConnection(monitor_connection);

			// 比赛开始，提前中断
			if (matchBegin) return;

			// 每次都重新获取设备的引用，此时可以 TODO 检查设备是否掉线

			if (auto& referee_sensor = *shared_referee_sensor; referee_sensor->CurrentMatchStatus ==
				RefereeData::MatchStatus::InMatch)
			{
				matchBegin = true;
				mind_status_package.Items[0].Value = 0;
				mind_status_package.Items[1].Value = 1;
				mind_status_package.Items[2].Value = 0;
				return;
			}

			matchBegin = false;
			mind_status_package.Items[0].Value = 1;
			mind_status_package.Items[1].Value = 0;
			mind_status_package.Items[2].Value = 0;

			// 执行其他策略
		},

		[&mind_status_package, &shared_full_data_sensor, &shared_referee_sensor, monitor_connection,
			shared_full_data_controller](std::atomic_bool& matchEnd)
		{
			mind_status_package.WriteJsonToSharedConnection(monitor_connection);

			// 比赛结束，提前中断
			if (matchEnd) return;

			// 获取传感器
			auto& device_sensor = *shared_full_data_sensor;
			auto& referee_sensor = *shared_referee_sensor;
			auto& device_controller = *shared_full_data_controller;

			// 比赛是否结束
			if (referee_sensor->CurrentMatchStatus == RefereeData::MatchStatus::AfterMatch)
			{
				matchEnd = true;
				mind_status_package.Items[0].Value = 0;
				mind_status_package.Items[1].Value = 0;
				mind_status_package.Items[2].Value = 1;
				return;
			}
			mind_status_package.Items[0].Value = 0;
			mind_status_package.Items[1].Value = 1;
			mind_status_package.Items[2].Value = 0;

			// 寻找装甲板
			const auto latest_armor_unit_id = device_sensor->LatestArmorPlate.ID;
			const bool find_valid_armor = latest_armor_unit_id != 0;
			mind_status_package.Items[3].Value = find_valid_armor ? static_cast<long long>(latest_armor_unit_id) : 0;

			// TODO 检查弹药

			// 攻击装甲板
			const bool attack_valid_armor = find_valid_armor && latest_armor_unit_id != 2;
			device_controller->HitTarget = static_cast<long long>((attack_valid_armor
				                                                       ? latest_armor_unit_id
				                                                       : 0));
			mind_status_package.Items[4].Value = device_controller->HitTarget;

			// 去前哨
			const auto current_outpost_health = referee_sensor->OutpostHealth;
			const bool outpost_destroy = current_outpost_health == 0;
			device_controller->MoveDestination = outpost_destroy ? 1 : 0;
			mind_status_package.Items[5].Value = device_controller->MoveDestination;

			// 执行其他策略
		},

		[&mind_status_package, shared_referee_sensor, monitor_connection](std::atomic_bool& matchReset)
		{
			mind_status_package.WriteJsonToSharedConnection(monitor_connection);

			// 比赛重置，提前中断
			if (matchReset) return;

			// 每次都重新获取设备的引用，此时可以 TODO 检查设备是否掉线
			if (auto& referee_sensor = *shared_referee_sensor; referee_sensor->CurrentMatchStatus ==
				RefereeData::MatchStatus::BeforeMatch)
			{
				matchReset = true;
				mind_status_package.Items[0].Value = 1;
				mind_status_package.Items[1].Value = 0;
				mind_status_package.Items[2].Value = 0;
				return;
			}
			mind_status_package.Items[0].Value = 0;
			mind_status_package.Items[1].Value = 0;
			mind_status_package.Items[2].Value = 1;

			// 执行其他策略
		}
	};
	match_status_manager.LoopRun();

	return 0;
}
