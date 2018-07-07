#pragma once
#include "Triangle.h"
#include "Vector.h"
#include "Matrix.h"
#include "Plane.h"


class Camera
{
public:
	Camera(Math::Vec4 cameraDirection, Math::Vec4 worldPosition, float nearZ, float farZ, float fov, float viewportWidth, float viewportHeight);

public:
	void CullObject();
	void CullBack(Triangle& triangle);

	void cameraToPerspective(Math::Vec4* position);
	void perspectiveToScreen(Math::Vec4* position);

	void calWorldToCameraMatrix();

	Math::Vec4 cameraDirection;
private:
	Math::Vec4 worldPosition;

	Plane leftClipPlane;
	Plane rightClipPlane;
	Plane topClipPlane;
	Plane bottomClipPlane;

	float nearZ;
	float farZ;

	//½Ç¶È
	float fov;

	float viewportWidth;
	float viewportHeight;
	float viewportCenterX;
	float viewportCenterY;
	float viewplaneWidth;
	float viewplaneHeight;

	float viewplaneDistance;

	//ÆÁÄ»¿í¸ß±È
	float aspectRatio;

public:
	Math::Matrix4 worldToCameraMatrix;
	Math::Matrix4 cameraToPerspectiveMatrix;
	Math::Matrix4 perspectiveToScreenMatrix;

};
