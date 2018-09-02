#include "Matrix.h"
#include "Vector.h"
#include "MathUtil.h"


Matrix2::Matrix2()
{
	for (int i = 0; i < 4; i++)
		data[i] = 0.0f;
}

Matrix2::Matrix2(float _m00, float _m01, float _m10, float _m11)
{
	this->_m00 = _m00;
	this->_m01 = _m01;
	this->_m10 = _m10;
	this->_m11 = _m11;
}

void Matrix2::Transpose()
{
	float temp = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < i; j++)
		{
			temp = data[j * 2 + i];
			data[j * 2 + i] = data[i * 2 + j];
			data[i * 2 + j] = temp;
		}
	}
}

Matrix2 Matrix2::operator*(float scale)
{
	return Matrix2(this->_m00 * scale, this->_m01 * scale, this->_m10 * scale, this->_m11 * scale);
}

Matrix2 Matrix2::operator*(const Matrix2 & right)
{
	Matrix2 result;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			float temp = 0.0f;
			for (int index = 0; index < 2; index++)
			{
				temp += this->data[i * 2 + index] * right.data[index * 2 + j];
			}
			result.data[i * 2 + j] = temp;
		}
	}
	return result;
}

float Matrix2::Determinant() const
{
	return this->_m00 * this->_m11 - this->_m01 * this->_m10;
}

Matrix3::Matrix3()
{
	for (int index = 0; index < 9; index++)
		this->data[index] = 0.0f;
}

Matrix3::Matrix3(float _m00, float _m01, float _m02, float _m10, float _m11, float _m12, float _m20, float _m21, float _m22)
{
	this->_m00 = _m00;
	this->_m01 = _m01;
	this->_m02 = _m02;
	this->_m10 = _m10;
	this->_m11 = _m11;
	this->_m12 = _m12;
	this->_m20 = _m20;
	this->_m21 = _m21;
	this->_m22 = _m22;
}


Matrix4::Matrix4()
{
	for (int index = 0; index < 16; index++)
		this->data[index] = 0.0f;
}

Matrix4::Matrix4(float _m00, float _m01, float _m02, float _m03, float _m10, float _m11, float _m12, float _m13, float _m20, float _m21, float _m22, float _m23, float _m30, float _m31, float _m32, float _m33)
{
	this->_m00 = _m00;
	this->_m01 = _m01;
	this->_m02 = _m02;
	this->_m03 = _m03;
	this->_m10 = _m10;
	this->_m11 = _m11;
	this->_m12 = _m12;
	this->_m13 = _m13;
	this->_m20 = _m20;
	this->_m21 = _m21;
	this->_m22 = _m22;
	this->_m23 = _m23;
	this->_m30 = _m30;
	this->_m31 = _m31;
	this->_m32 = _m32;
	this->_m33 = _m33;
}

void Matrix4::Identity()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				this->data[i * 4 + j] = 1.0f;
			else
				this->data[i * 4 + j] = 0.0f;
		}
	}
}

void Matrix4::LoadTranslate(const Vec3& translatePosition)
{
	this->_m00 = 1.0f;
	this->_m01 = 0.0f;
	this->_m02 = 0.0f;
	this->_m03 = 0.0f;
	this->_m10 = 0.0f;
	this->_m11 = 1.0f;
	this->_m12 = 0.0f;
	this->_m13 = 0.0f;
	this->_m20 = 0.0f;
	this->_m21 = 0.0f;
	this->_m22 = 1.0f;
	this->_m23 = 0.0f;
	this->_m30 = translatePosition.x;
	this->_m31 = translatePosition.y;
	this->_m32 = translatePosition.z;
	this->_m33 = 1.0f;
}

void Matrix4::LoadRotX(float theta)
{
	float cosValue = cos(DegToRad(theta));
	float sinValue = sin(DegToRad(theta));

	this->_m00 = 1.0f;
	this->_m01 = 0.0f;
	this->_m02 = 0.0f;
	this->_m03 = 0.0f;
	this->_m10 = 0.0f;
	this->_m11 = cosValue;
	this->_m12 = sinValue;
	this->_m13 = 0.0f;
	this->_m20 = 0.0f;
	this->_m21 = -sinValue;
	this->_m22 = cosValue;
	this->_m23 = 0.0f;
	this->_m30 = 0.0f;
	this->_m31 = 0.0f;
	this->_m32 = 0.0f;
	this->_m33 = 1.0f;
}

void Matrix4::LoadRotY(float theta)
{
	float cosValue = cos(DegToRad(theta));
	float sinValue = sin(DegToRad(theta));

	this->_m00 = cosValue;
	this->_m01 = 0.0f;
	this->_m02 = -sinValue;
	this->_m03 = 0.0f;
	this->_m10 = 0.0f;
	this->_m11 = 1.0f;
	this->_m12 = 0.0f;
	this->_m13 = 0.0f;
	this->_m20 = sinValue;
	this->_m21 = 0.0f;
	this->_m22 = cosValue;
	this->_m23 = 0.0f;
	this->_m30 = 0.0f;
	this->_m31 = 0.0f;
	this->_m32 = 0.0f;
	this->_m33 = 1.0f;
}

void Matrix4::LoadRotZ(float theta)
{
	float cosValue = cos(DegToRad(theta));
	float sinValue = sin(DegToRad(theta));

	this->_m00 = cosValue;
	this->_m01 = sinValue;
	this->_m02 = 0.0f;
	this->_m03 = 0.0f;
	this->_m10 = -sinValue;
	this->_m11 = cosValue;
	this->_m12 = 0.0f;
	this->_m13 = 0.0f;
	this->_m20 = 0.0f;
	this->_m21 = 0.0f;
	this->_m22 = 1.0f;
	this->_m23 = 0.0f;
	this->_m30 = 0.0f;
	this->_m31 = 0.0f;
	this->_m32 = 0.0f;
	this->_m33 = 1.0f;
}

Matrix4 Matrix4::operator*(const Matrix4 & right) const
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float temp = 0.0f;
			for (int index = 0; index < 4; index++) 
			{
				temp += this->data[i * 4 + index] * right.data[index * 4 + j];
			}
			result.data[i * 4 + j] = temp;
		}
	}
	return result;
}

void Matrix4::operator*=(const Matrix4 & right)
{
	*this = *this * right;
}


