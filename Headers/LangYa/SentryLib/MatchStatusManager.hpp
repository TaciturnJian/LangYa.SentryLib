#pragma once

#include <atomic>
#include <functional>

#include <spdlog/spdlog.h>

namespace LangYa::SentryLib
{
	class MatchStatusManager final
	{
	public:
		enum MatchStatus
		{
			BeforeMatch,
			InMatch,
			AfterMatch
		};

		using MatchStatusCallback = std::function<void(std::atomic_bool&)>;

		static void EmptyCallback(std::atomic_bool&);

	private:
		volatile bool IsRunningLoop = false;
		std::atomic_bool IsLoopExited{false};
		MatchStatus MyStatus{BeforeMatch};
		MatchStatusCallback BeforeMatchCallback{EmptyCallback};
		MatchStatusCallback InMatchCallback{EmptyCallback};
		MatchStatusCallback AfterMatchCallback{EmptyCallback};

		void BeforeMatchLoop();

		void InMatchLoop();

		void AfterMatchLoop();

	public:
		MatchStatusManager();

		MatchStatusManager(
			MatchStatusCallback beforeMatch, 
			MatchStatusCallback inMatch,
		    MatchStatusCallback afterMatch
		);

		MatchStatusManager(const MatchStatusManager&) = delete;
		MatchStatusManager(MatchStatusManager&&) = delete;

		MatchStatusCallback& operator[](MatchStatus status);

		const MatchStatusCallback& operator[](MatchStatus status) const;

		void LoopRun();

		void Stop();
	};
}
