# SentryLib 代码文档

>[!tip] 如果你不能打开下面的链接，请尝试在 [GitHub](https://github.com/TaciturnJian/LangYa.SentryLib.git) 中打开。

![HUST](https://img.shields.io/badge/RoboMaster-华中科技大学狼牙战队-blue)
![Version](https://img.shields.io/badge/版本-1.0.1-blue)
![CXXStandard](https://img.shields.io/badge/CXX-20-green)
![CMakeVersion](https://img.shields.io/badge/CMAKE-3.16-green)

SentryLib 是一个机器人工具库，它包含了如下功能或接口：

- [内存管理（缓冲区，字节保护）](#内存管理工具)
- [序列化与反序列化接口](#序列化与反序列化)
- [连接接口及串口连接、TCP套接字连接实现](#连接接口)
- [机器人设备：控制器和传感器](#机器人设备)
- [一些简单的数学概念，便于表示一些数据](#数学概念)

未来规划

- [TODO List](#TODOList)

## 内存管理工具

- [内存视图 MemoryView.hpp](./Headers/LangYa/SentryLib/MemoryView.hpp)
- [内存输入输出 MemoryIO.hpp](./Headers/LangYa/SentryLib/MemoryIO.hpp)
- [独有内存 UniqueBuffer.hpp](./Headers/LangYa/SentryLib/UniqueBuffer.hpp)
- [资源管理 Resource.hpp](./Headers/LangYa/SentryLib/Resource.hpp)
- [二重字节缓冲 PingPongBuffer.hpp](./Headers/LangYa/SentryLib/PingPongBuffer.hpp)
- [三重内容缓冲 TripleBuffer.hpp](./Headers/LangYa/SentryLib/TripleBuffer.hpp)

## 序列化与反序列化

- [可序列化内容 SerializableContent.hpp](./Headers/LangYa/SentryLib/SerializableContent.hpp)
- [可序列化概念 SerializableContent.hpp](./Headers/LangYa/SentryLib/SerializableContent.hpp)
- [可反序列化内容 DeserializableContent.hpp](./Headers/LangYa/SentryLib/SerializableContent.hpp)
- [可反序列化概念 DeserializableContent.hpp](./Headers/LangYa/SentryLib/SerializableContent.hpp)

## 连接接口

- [连接 Connection.hpp](./Headers/LangYa/SentryLib/Connection.hpp)
- [串口信息 SerialPortInfo.hpp](./Headers/LangYa/SentryLib/SerialPortInfo.hpp)
- [串口连接 SerialPort.hpp](./Headers/LangYa/SentryLib/SerialPort.hpp)
- [网络连接 TCPConnection.hpp](./Headers/LangYa/SentryLib/TCPConnection.hpp)
- [狼牙连接 LangYaConnection.hpp](./Headers/LangYa/SentryLib/LangYaConnection.hpp)
- [内存访问 MemoryAccess.hpp](./Headers/LangYa/SentryLib/MemoryAccess.hpp)

## 数学概念

- 位置
    - [一维位置 Position1](./Headers/LangYa/SentryLib/Position1.hpp)
    - [二维位置 Position2](./Headers/LangYa/SentryLib/Position2.hpp)
    - [三维位置 Position3](./Headers/LangYa/SentryLib/Position3.hpp)
- 向量
    - [向量 Vector](./Headers/LangYa/SentryLib/Vector.hpp)
    - [二维向量 Vector2](./Headers/LangYa/SentryLib/Vector2.hpp)
    - [三维向量 Vector3](./Headers/LangYa/SentryLib/Vector3.hpp)

## 机器人设备

- [设备 Device.hpp](./Headers/LangYa/SentryLib/Device.hpp)
- [传感器 Sensor.hpp](./Headers/LangYa/SentryLib/Sensor.hpp)
- [控制器 Controller.hpp](./Headers/LangYa/SentryLib/Controller.hpp)


# TODOList

- [x] 移动到 `LangYa::SentryLib`（修改所有文件）
    - [x] 把ToBeManaged所有头文件都放在 ./Headers/LangYa/Sentry/ 下
    - [x] 把ToBeManaged所有源文件都放在 ./Sources/ 下
    - [x] 修改所有文件的命名空间
- [ ] 检查源文件问题
    - [ ] 多线程问题检查
    - [x] 内存泄漏检查
    - [x] 静态函数优化
- [x] 头文件代码注释
- [x] 使用方法
- [ ] 优化代码结构
- [ ] 记录所有依赖库，其他库只要链接此库就会自动链接依赖库
- [ ] GitHub 代码管理


