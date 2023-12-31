#include <LangYa/SentryLib/Interface/ArmorPlate.hpp>

namespace LangYa::SentryLib
{
	ArmorPlate::ArmorPlate() = default;

	ArmorPlate::ArmorPlate(const char id, const ArmorColor color, PositionedVector3F particle):
		ID(id),
		Color(color),
		Particle(std::move(particle))
	{
	}

	std::ostream& ArmorPlate::FormatToJsonString(std::ostream& stream) const
	{
		return stream << "{"
			<< R"("ID":)" << static_cast<int>(ID) << ','
			<< R"("Color":)" << static_cast<int>(Color) << ','
			<< R"("Position":)" << Particle.Position << ','
			<< R"("NormalDirection":)" << Particle.Direction
			<< "}";
	}
}
