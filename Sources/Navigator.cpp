#include <LangYa/SentryLib/Interface/Navigator.hpp>

namespace LangYa::SentryLib
{
	Navigator::~Navigator() = default;

	bool
	Navigator::GetSelfLocation(TinyMath::Position3F& selfLocation)
	{
		return false;
	}

	bool
	Navigator::GetVelocityToDestination(const DestinationIDType& destination, TinyMath::Vector2F& velocity)
	{
		return false;
	}

	bool
	Navigator::GetObstaclePosition(std::vector<TinyMath::Position2F>& obstacles)
	{
		return false;
	}
}
