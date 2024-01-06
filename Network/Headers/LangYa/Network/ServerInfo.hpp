#pragma once

#include <atomic>
#include <cstdint>
#include <memory>

#include <LangYa/Network/IClientAcceptor.hpp>
#include <LangYa/Network/IClientHandler.hpp>

namespace LangYa::Network
{
	template <typename TClient>
	struct ServerInfo
	{
		std::shared_ptr<IClientAcceptor<TClient>> AcceptorPointer;
		std::shared_ptr<IClientHandler<TClient>> HandlerPointer;
		std::shared_ptr<std::atomic_bool> InterruptSignalPointer;
		std::uint64_t MaxAcceptorFailureCount{5};
	};
}
