#pragma once
#include "Vector.h"
#include "Matrix.h"


class VertexOut;

inline float DegToRad(float deg) { return deg / 180 * 3.1415926; }

VertexOut clamp(VertexOut& left, VertexOut& right, float t);
Math::Vec4 clamp(Math::Vec4 min, Math::Vec4 max, float t);
Math::Vec3 clamp(Math::Vec3 min, Math::Vec3 max, float t);
Math::Vec2 clamp(Math::Vec2 min, Math::Vec2 max, float t);
float clamp(float min, float max, float t);





Math::Matrix4 translateToMatrix(const Math::Vec3& worldPosition);
Math::Matrix4 eularToMatrix(const Math::Vec3& eular);
Math::Matrix4 GetMatrixByTranslateAndEular(Math::Vec3 worldPosition, Math::Vec3 eular);

Math::Matrix4 GetModelMatrix(Math::Vec3 position, Math::Vec3 eular);
Math::Matrix4 GetViewMatrix(Math::Vec3 position, Math::Vec3 eular);
Math::Matrix4 GetPerspectiveMatrix(float fovx, float aspect, float zn, float zf);

Math::Vec3 Cross(const Math::Vec3& left, const Math::Vec3& right);
Math::Vec3 GetForward(float yAngle);
