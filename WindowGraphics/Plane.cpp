#include "Plane.h"

void Plane::Set(const Math::Vec3 & normal, const Math::Vec3& origin)
{
	this->normal = normal;
	this->origin = origin;
}
