#pragma once
#include "Vector.h"
#include "Matrix.h"

class Camera
{
public:
	Camera();
	Camera(Math::Vec3 cameraDirection, Math::Vec3 worldPosition, float nearZ, float farZ, float fovx, float viewportWidth, float viewportHeight);
	~Camera();

public:
	void CalWorldToCameraMatrix();

public:
	Math::Vec3 cameraDirection;
	Math::Vec3 worldPosition;

	float nearZ;
private:
	float farZ;

	//�Ƕ�
	float fovx;

	float viewportWidth;
	float viewportHeight;

	float viewplaneWidth;
	float viewplaneHeight;

	float viewplaneDistance;

	//��Ļ��߱�
	float aspectRatio;

};
