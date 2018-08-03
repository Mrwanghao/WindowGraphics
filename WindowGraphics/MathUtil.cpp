#include "MathUtil.h"
#include "Vertex.h"
#include "Camera.h"
#include <math.h>

using namespace Math;

extern Camera mainCamera;
extern Matrix4 modelMatrix, viewMatrix, projectMatrix;


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
#if 0
	Vec4 lerp = min + (max - min) * t;
	lerp.w = min.w;
	return lerp;
#else
	return min + (max - min) * t;
#endif
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
	modelMatrix = GetMatrixByTranslateAndEular(position, eular);
	return modelMatrix;
}

Math::Matrix4 GetViewMatrix(Vec3 cameraPosition, Vec3 cameraDirection)
{
	viewMatrix = GetMatrixByTranslateAndEular(-cameraPosition, -cameraDirection);
	return viewMatrix;
}

Math::Matrix4 GetPerspectiveMatrix(float fovx, float aspect, float zn, float zf)
{
	projectMatrix.Identity();

	float rad = DegToRad(fovx / 2.0f);
	float cotan_fovx = 1.0f / tan(rad);

	projectMatrix._data[0] = cotan_fovx;
	projectMatrix._data[5] = cotan_fovx * aspect;
	projectMatrix._data[10] = zf / (zf - zn);
	projectMatrix._data[11] = 1.0f;
	projectMatrix._data[14] = - zn * zf / (zf - zn);
	projectMatrix._data[15] = 0.0f;

	return projectMatrix;
}

Math::Vec3 GetForward(float yAngle)
{
	float rad = DegToRad(yAngle + 90);
	return Math::Vec3(cosf(rad), 0.0f, sinf(rad));
}

#pragma endregion
