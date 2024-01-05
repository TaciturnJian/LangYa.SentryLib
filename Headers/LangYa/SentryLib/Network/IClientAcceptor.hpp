#pragma once

#include <memory>

namespace LangYa::SentryLib
{
	/// @brief 代表可以接收客户端的类。
	///	@details 该类的实现类可以接收客户端的连接请求，并且在同一时间最多返回一个有效客户端。
	///	通过反复调用 AcceptClient 即可实现接收多个客户端连接。
	///	@tparam TClient 客户端类型。
	template<typename TClient>
	class IClientAcceptor
	{
	public:
		virtual ~IClientAcceptor() = default;

		/// @brief 处理客户端连接。
		virtual std::shared_ptr<TClient> AcceptClient() = 0; 
	};
}