#pragma once
#include "Vector.h"
#include "Matrix.h"

inline float DegToRad(float deg) { return deg / 180 * 3.1415926; }

Math::Vec3 Cross(const Math::Vec3& left, const Math::Vec3& right);
Math::Matrix4 eularToMatrix(const Math::Vec3& eular);

