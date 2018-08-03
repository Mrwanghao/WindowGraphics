#include "Camera.h"
#include "MathUtil.h"

using namespace Math;

extern Matrix4 viewMatrix;

Camera::Camera()
{
}

Camera::Camera(Math::Vec3 cameraDirection, Math::Vec3 worldPosition, float nearZ, float farZ, float fovx, float viewportWidth, float viewportHeight)
	:
	cameraDirection(cameraDirection),
	worldPosition(worldPosition),
	nearZ(nearZ),
	farZ(farZ),
	fovx(fovx),
	viewportWidth(viewportWidth),
	viewportHeight(viewplaneHeight)
{
	this->aspectRatio = (float)viewportWidth / (float)viewportHeight;

	this->viewplaneWidth = 2.0f;
	this->viewplaneHeight = 2.0f / this->aspectRatio;

	float tan_fov_inv = 1.0f / tan(DegToRad(fovx * 0.5f));
	this->viewplaneDistance = this->viewplaneWidth * 0.5f * tan_fov_inv;
}

Camera::~Camera()
{
}

void Camera::CalWorldToCameraMatrix()
{
	viewMatrix = GetViewMatrix(worldPosition, cameraDirection);
}

Math::Vec3 Camera::GetCameraForward()
{
	return GetForward(cameraDirection.y);
}

