#pragma once

namespace LangYa::SentryLib {
	template<typename TClient>
	class IClientHandler {
	public:
		struct HandleClientArgumentType final
		{
			std::shared_ptr<TClient> ClientPointer{nullptr};
			std::shared_ptr<std::atomic_bool> InterruptSignalPointer{nullptr};
		};

		virtual ~IClientHandler() = default;
		virtual void HandleClient(const HandleClientArgumentType& argument) = 0;
	};
}