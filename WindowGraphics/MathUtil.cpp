#include "MathUtil.h"
#include "Vertex.h"
#include "Camera.h"
#include <math.h>

using namespace Math;

extern Camera mainCamera;
//extern Matrix4 modelMatrix, viewMatrix, projectMatrix;


VertexOut clamp(VertexOut& left, VertexOut& right, float t)
{
	VertexOut result;

	result.clipPosition = clamp(left.clipPosition, right.clipPosition, t);
	result.viewPosition = clamp(left.viewPosition, right.viewPosition, t);
	result.worldPosition = clamp(left.worldPosition, right.worldPosition, t);
	result.worldNormal = clamp(left.worldNormal, right.worldNormal, t);
	result.uv = clamp(left.uv, right.uv, t);
											
	return result;
}

Math::Vec4 clamp(Math::Vec4 min, Math::Vec4 max, float t)
{
	return min + (max - min) * t;
}

Vec3 clamp(Vec3 min, Vec3 max, float t)
{
	return min + (max - min) * t;
}

Math::Vec2 clamp(Math::Vec2 min, Math::Vec2 max, float t)
{
	return min + (max - min) * t;
}

float clamp(float min, float max, float t)
{
	return min + (max - min) * t;
}

Math::Vec3 Cross(const Math::Vec3 & left, const Math::Vec3 & right)
{
	return Math::Vec3(
		left.y * right.z - right.y * left.z,
		left.x * right.z - right.x * left.z, 
		left.x * right.y - right.x * left.y);
}

#pragma region 坐标或者方向的矩阵变换
Math::Matrix4 translateToMatrix(const Math::Vec3 & worldPosition)
{
	Matrix4 mtInvMatrix;
	mtInvMatrix.LoadTranslate(worldPosition);
	return mtInvMatrix;
}

Math::Matrix4 eularToMatrix(const Math::Vec3 & eular)
{
	// vec4 xmatrix ymatrix zmatrix translatematrix
	// x y z
	Math::Matrix4 mTmpInvMatrix, mRotIncMatrix;

	mTmpInvMatrix.LoadRotX(eular.x);
	mRotIncMatrix  = mTmpInvMatrix;

	mTmpInvMatrix.LoadRotY(eular.y);
	mRotIncMatrix *= mTmpInvMatrix;

	mTmpInvMatrix.LoadRotZ(eular.z);
	mRotIncMatrix *= mTmpInvMatrix;

	return mRotIncMatrix;
}

Math::Matrix4 GetMatrixByTranslateAndEular(Math::Vec3 worldPosition, Math::Vec3 eular)
{
	Matrix4 mtInvMatrix = translateToMatrix(worldPosition);
	Matrix4 mRotInvMatrix = eularToMatrix(eular);

	return mtInvMatrix * mRotInvMatrix;
}

Math::Matrix4 GetModelMatrix(Math::Vec3 position, Math::Vec3 eular)
{
	return GetMatrixByTranslateAndEular(position, eular);
}

Math::Matrix4 GetViewMatrix(Vec3 cameraPosition, Vec3 cameraDirection)
{
	return GetMatrixByTranslateAndEular(-cameraPosition, -cameraDirection);
}

Math::Matrix4 GetPerspectiveMatrix(float fovx, float aspect, float zn, float zf)
{
	Matrix4 result;
	result.Identity();

	float rad = DegToRad(fovx / 2.0f);
	float cotan_fovx = 1.0f / tan(rad);

	result.data[0] = cotan_fovx;
	result.data[5] = cotan_fovx * aspect;
	result.data[10] = zf / (zf - zn);
	result.data[11] = 1.0f;
	result.data[14] = - zn * zf / (zf - zn);
	result.data[15] = 0.0f;

	return result;
}

Math::Vec3 GetForward(float yAngle)
{
	float rad = DegToRad(yAngle);
	return Math::Vec3(sinf(rad), 0.0f, cosf(rad));
}

#pragma endregion
