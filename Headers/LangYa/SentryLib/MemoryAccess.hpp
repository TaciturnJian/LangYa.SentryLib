#pragma once

#include <LangYa/SentryLib/MemoryView.hpp>
#include <LangYa/SentryLib/Connection.hpp>

namespace LangYa::SentryLib
{
	class MemoryAccess final : public Connection
	{
		MemoryView View;

	public:
		explicit MemoryAccess(MemoryView view);

		void Connect() override;
		bool IsConnected() override;
		void Disconnect() override;

		MemoryView::SizeType Read(const MemoryView& view) override;

		MemoryView::SizeType Write(const MemoryView& view) override;
	};
}
