#pragma once
#include "Vector.h"
#include "Matrix.h"

class Camera
{
public:
	Camera();
	Camera(Vec3 cameraDirection, Vec3 worldPosition, float nearZ, float farZ, float fovx, float viewportWidth, float viewportHeight);
	~Camera();

public:
	void CalWorldToCameraMatrix();
	Vec3 GetCameraForward();
	Matrix4 GetProjectionMatrix();

public:
	Vec3 cameraDirection;
	Vec3 worldPosition;

public:
	//½Ç¶È
	float fovx;

	float viewportWidth;
	float viewportHeight;

	float viewplaneWidth;
	float viewplaneHeight;

	float viewplaneDistance;
	float nearZ;
	float farZ;
	//ÆÁÄ»¿í¸ß±È
	float aspectRatio;

};
