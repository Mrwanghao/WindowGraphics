#pragma once
#include "Vector.h"
#include "Matrix.h"


#define PI 3.14159265358979f

class VertexOut;

inline float DegToRad(float deg) { return deg / 180.0f * PI; }

VertexOut clamp(VertexOut& left, VertexOut& right, float t);
Vec4 clamp(Vec4 min, Vec4 max, float t);
Vec3 clamp(Vec3 min, Vec3 max, float t);
Vec2 clamp(Vec2 min, Vec2 max, float t);
float clamp(float min, float max, float t);





Matrix4 translateToMatrix(const Vec3& worldPosition);
Matrix4 eularToMatrix(const Vec3& eular);
Matrix4 GetMatrixByTranslateAndEular(Vec3 worldPosition, Vec3 eular);

Matrix4 GetModelMatrix(Vec3 position, Vec3 eular);
Matrix4 GetViewMatrix(Vec3 position, Vec3 eular);
Matrix4 GetPerspectiveMatrix(float fovx, float aspect, float zn, float zf);

Vec3 Cross(const Vec3& left, const Vec3& right);
Vec3 GetForward(float yAngle);
