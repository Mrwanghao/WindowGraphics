#pragma once
#include "Vector.h"


class Plane
{
private:
	Math::Vec3 normal;
	Math::Vec3 origin;

public:
	void Set(const Math::Vec3& normal, const Math::Vec3& origin);

};
