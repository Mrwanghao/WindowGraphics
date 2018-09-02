#include "MathUtil.h"
#include "Vertex.h"
#include "Camera.h"
#include <math.h>



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
	result.onePerZ = clamp(left.onePerZ, right.onePerZ, t);
	return result;
}

Vec4 clamp(Vec4 min, Vec4 max, float t)
{
	return min + (max - min) * t;
}

Vec3 clamp(Vec3 min, Vec3 max, float t)
{
	return min + (max - min) * t;
}

Vec2 clamp(Vec2 min, Vec2 max, float t)
{
	return min + (max - min) * t;
}

float clamp(float min, float max, float t)
{
	return min + (max - min) * t;
}

Vec3 Cross(const Vec3 & left, const Vec3 & right)
{
	return Vec3(
		left.y * right.z - right.y * left.z,
		left.x * right.z - right.x * left.z, 
		left.x * right.y - right.x * left.y);
}

#pragma region 坐标或者方向的矩阵变换
Matrix4 translateToMatrix(const Vec3 & worldPosition)
{
	Matrix4 mtInvMatrix;
	mtInvMatrix.LoadTranslate(worldPosition);
	return mtInvMatrix;
}

Matrix4 eularToMatrix(const Vec3 & eular)
{
	// vec4 xmatrix ymatrix zmatrix translatematrix
	// x y z
	Matrix4 mTmpInvMatrix, mRotIncMatrix;

	mTmpInvMatrix.LoadRotX(eular.x);
	mRotIncMatrix  = mTmpInvMatrix;

	mTmpInvMatrix.LoadRotY(eular.y);
	mRotIncMatrix *= mTmpInvMatrix;

	mTmpInvMatrix.LoadRotZ(eular.z);
	mRotIncMatrix *= mTmpInvMatrix;

	return mRotIncMatrix;
}

Matrix4 GetMatrixByTranslateAndEular(Vec3 worldPosition, Vec3 eular)
{
	Matrix4 mtInvMatrix = translateToMatrix(worldPosition);
	Matrix4 mRotInvMatrix = eularToMatrix(eular);

	return mtInvMatrix * mRotInvMatrix;
}

Matrix4 GetModelMatrix(Vec3 position, Vec3 eular)
{
	return GetMatrixByTranslateAndEular(position, eular);
}

Matrix4 GetViewMatrix(Vec3 cameraPosition, Vec3 cameraDirection)
{
	return GetMatrixByTranslateAndEular(-cameraPosition, -cameraDirection);
}

Matrix4 GetPerspectiveMatrix(float fovx, float aspect, float zn, float zf)
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

Vec3 GetForward(float yAngle)
{
	float rad = DegToRad(yAngle);
	return Vec3(sinf(rad), 0.0f, cosf(rad));
}

#pragma endregion
