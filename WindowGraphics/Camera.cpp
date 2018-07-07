#include "Camera.h"
#include "MathUtil.h"

using namespace Math;

Camera::Camera(Math::Vec4 cameraDirection, Math::Vec4 worldPosition, float nearZ, float farZ, float fov, float viewportWidth, float viewportHeight)
	:
	cameraDirection(cameraDirection),
	worldPosition(worldPosition),
	nearZ(nearZ),
	farZ(farZ),
	fov(fov)
{

	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	this->viewportCenterX = (viewportWidth - 1) * 0.5f;
	this->viewportCenterY = (viewportHeight - 1) * 0.5f;
	
	this->aspectRatio = (float)viewportWidth / (float)viewportHeight;

	this->viewplaneWidth = 2.0f;
	this->viewplaneHeight = 2.0f / this->aspectRatio;

	float tan_fov_inv = 1.0f / tan(DegToRad(fov * 0.5f));
	//float tan_fov_inv = tan(DegToRad(fov * 0.5f));
	this->viewplaneDistance = this->viewplaneWidth * 0.5f * tan_fov_inv;

	this->worldToCameraMatrix.Identity();
	this->cameraToPerspectiveMatrix.Identity();
	this->perspectiveToScreenMatrix.Identity();

	Vec3 origin;//裁剪面上的一个点
	origin.Set(0.0f, 0.0f, 0.0f);
	Vec3 normal;//裁剪面的法线

	if (fov == 90.0f)
	{
		normal.Set(1.0f, 0.0f, -1.0f);
		rightClipPlane.Set(normal, origin);

		normal.Set(-1.0f, 0.0f, 1.0f);
		leftClipPlane.Set(normal, origin);

		normal.Set(0.0f, 1.0f, -1.0f);
		topClipPlane.Set(normal, origin);

		normal.Set(0.0f, -1.0f, -1.0f);
		bottomClipPlane.Set(normal, origin);

	}
	else
	{
		normal.Set( this->viewplaneDistance, 0.0f, - viewplaneWidth / 2.0f);
		rightClipPlane.Set(normal, origin);

		normal.Set(-this->viewplaneDistance, 0.0f, -viewplaneWidth / 2.0f);
		leftClipPlane.Set(normal, origin);

		normal.Set(0.0f,  this->viewplaneDistance, -viewplaneWidth / 2.0f);
		topClipPlane.Set(normal, origin);

		normal.Set(0.0f, -this->viewplaneDistance, -viewplaneWidth / 2.0f);
		bottomClipPlane.Set(normal, origin);
	}

}

void Camera::CullObject()
{

}

void Camera::CullBack(Triangle& triangle)
{
	
}

void Camera::calWorldToCameraMatrix()
{
	Math::Matrix4 mtInvMatrix;
	mtInvMatrix.LoadTranslate(this->worldPosition);

	// vec4 xmatrix ymatrix zmatrix translatematrix
	// x y z
	Math::Matrix4 mTmpInvMatrix, mRotIncMatrix;
	mTmpInvMatrix.LoadRotX(this->cameraDirection._x);
	mRotIncMatrix = mTmpInvMatrix;

	mTmpInvMatrix.LoadRotY(this->cameraDirection._y);
	mRotIncMatrix *= mTmpInvMatrix;

	mTmpInvMatrix.LoadRotZ(this->cameraDirection._z);
	mRotIncMatrix *= mTmpInvMatrix;

	this->worldToCameraMatrix = mRotIncMatrix * mtInvMatrix;
}

void Camera::cameraToPerspective(Math::Vec4* position)
{
	float z = position->_z;
	position->_x = this->viewplaneDistance * position->_x / z;
	position->_y = (float)(this->viewplaneDistance * position->_y * this->aspectRatio / z);
	position->_z = z;
	position->_w = 1.0f;
}

void Camera::perspectiveToScreen(Math::Vec4* position)
{
	float alpha = 0.5f * this->viewportWidth - 0.5f;
	float beta = 0.5f * this->viewportHeight - 0.5f;

	position->_x = alpha + alpha * position->_x;
	position->_y = beta  - beta  * position->_y;

}