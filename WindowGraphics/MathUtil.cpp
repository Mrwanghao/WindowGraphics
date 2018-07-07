#include "MathUtil.h"



Math::Vec3 Cross(const Math::Vec3 & left, const Math::Vec3 & right)
{
	return Math::Vec3(
		left._y * right._z - right._y * left._z,
		left._x * right._z - right._x * left._z, 
		left._x * right._y - right._x * left._y);
}

Math::Matrix4 eularToMatrix(const Math::Vec3 & eular)
{

	// vec4 xmatrix ymatrix zmatrix translatematrix
	// x y z
	Math::Matrix4 mTmpInvMatrix, mRotIncMatrix;
	mTmpInvMatrix.LoadRotX(eular._x);
	mRotIncMatrix = mTmpInvMatrix;

	mTmpInvMatrix.LoadRotY(eular._y);
	mRotIncMatrix *= mTmpInvMatrix;

	mTmpInvMatrix.LoadRotZ(eular._z);
	mRotIncMatrix *= mTmpInvMatrix;

	return mRotIncMatrix;
}
