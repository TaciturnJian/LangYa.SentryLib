#include <LangYa/SentryLib/MatchStatusManager.hpp>

namespace LangYa::SentryLib
{
	void
	MatchStatusManager
	::EmptyCallback(std::atomic_bool&)
	{
	}

	void
	MatchStatusManager
	::BeforeMatchLoop()
	{
		std::atomic_bool is_match_begin{false};

		while (IsRunningLoop)
		{
			if (is_match_begin)
			{
				MyStatus = InMatch;
				break;
			}

			BeforeMatchCallback(is_match_begin);
		}
	}

	void
	MatchStatusManager
	::InMatchLoop()
	{
		std::atomic_bool is_match_end{false};

		while (IsRunningLoop)
		{
			if (is_match_end)
			{
				MyStatus = AfterMatch;
				break;
			}

			AfterMatchCallback(is_match_end);
		}
	}

	void
	MatchStatusManager
	::AfterMatchLoop()
	{
		std::atomic_bool is_match_restart{false};

		while (IsRunningLoop)
		{
			if (is_match_restart)
			{
				MyStatus = BeforeMatch;
				break;
			}

			AfterMatchCallback(is_match_restart);
		}
	}

	MatchStatusManager
	::MatchStatusManager()
	= default;

	MatchStatusManager
	::MatchStatusManager(
		MatchStatusCallback beforeMatch, 
		MatchStatusCallback inMatch,
		MatchStatusCallback afterMatch
	) :
		BeforeMatchCallback(std::move(beforeMatch)),
		InMatchCallback(std::move(inMatch)),
		AfterMatchCallback(std::move(afterMatch))
	{
	}

	MatchStatusManager::MatchStatusCallback& 
	MatchStatusManager
	::operator[](const MatchStatus status)
	{
		switch (status)
		{
		case BeforeMatch:
			return BeforeMatchCallback;

		case InMatch:
			return InMatchCallback;

		case AfterMatch:
			return AfterMatchCallback;

		default:                    // NOLINT(clang-diagnostic-covered-switch-default)
			throw std::exception(); //TODO 确定错误原因
		}
	}

	const MatchStatusManager::MatchStatusCallback& 
	MatchStatusManager::operator[](const MatchStatus status) const
	{
		switch (status)
		{
		case BeforeMatch:
			return BeforeMatchCallback;

		case InMatch:
			return InMatchCallback;

		case AfterMatch:
			return AfterMatchCallback;

		default:                    // NOLINT(clang-diagnostic-covered-switch-default)
			throw std::exception(); //TODO 确定错误原因
		}
	}

	void
	MatchStatusManager
	::LoopRun()
	{
		if (IsRunningLoop)
		{
			spdlog::info("MatchStatusManager> Trying to run loop when another loop is running!");
			return;
		}

		IsLoopExited = false;
		IsRunningLoop = true;
		while (IsRunningLoop)
		{
			switch (MyStatus)
			{
			case BeforeMatch:
				BeforeMatchLoop();
				break;

			case InMatch:
				InMatchLoop();
				break;

			case AfterMatch:
				AfterMatchLoop();
				break;

			default:                    // NOLINT(clang-diagnostic-covered-switch-default)
				throw std::exception(); //TODO 确定错误原因
			}
		}
		IsLoopExited = true;
	}

	void
	MatchStatusManager
	::Stop()
	{
		IsRunningLoop = false;
		while (!IsLoopExited)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		IsLoopExited = false;
	}
}
