#pragma once

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/Common/MemoryIO.hpp>

namespace LangYa::SentryLib
{
	class IOConnection : public MemoryIO
	{
	public:
		/// @brief 打开此连接，成功打开后便可以开始读写。
		///	@details 在未开启连接之前，调用 Read 和 Write 方法是未定义行为。
		///	调用此函数往往会阻塞当前线程，直到连接建立成功返回 true 或判断无法建立连接后返回 false 。
		virtual bool Open() = 0;

		/// @brief 关闭此连接，保证绝对成功关闭，即使抛出异常。
		virtual void Close() = 0;

		/// @brief 获取信息，将信息写入到流中。
		virtual std::ostream& GetTargetInfo(std::ostream& stream) = 0;
	};
}
