#include <LangYa/SentryLib/Interface/Navigator.hpp>

namespace LangYa::SentryLib
{
	Navigator::~Navigator() = default;

	bool
	Navigator::GetSelfLocation(Position3F& selfLocation)
	{
		return false;
	}

	bool
	Navigator::GetVelocityToDestination(const DestinationIDType& destination, Vector2F& velocity)
	{
		return false;
	}

	bool
	Navigator::GetObstaclePosition(std::vector<Position2F>& obstacles)
	{
		return false;
	}
}
