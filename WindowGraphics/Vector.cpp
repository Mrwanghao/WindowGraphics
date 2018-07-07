#include "Vector.h"
#include "Matrix.h"

namespace Math
{
	Vec2::Vec2()
	{
		this->_x = 0.0f;
		this->_y = 0.0f;
	}

	Vec2::Vec2(float _x, float _y)
	{
		this->_x = _x;
		this->_y = _y;
	}

	Vec2::Vec2(const Vec2 & right)
	{
		this->_x = right._x;
		this->_y = right._y;
	}

	void Vec2::Set(float _x, float _y)
	{
		this->_x = _x;
		this->_y = _y;
	}

	void Vec2::SetX(float _x)
	{
		this->_x = _x;
	}

	void Vec2::SetY(float _y)
	{
		this->_y = _y;
	}

	float Vec2::GetX()
	{
		return this->_x;
	}

	float Vec2::GetY()
	{
		return this->_y;
	}

	void Vec2::LoadZero()
	{
		this->_x = this->_y = 0.0f;
	}

	void Vec2::LoadOne()
	{
		this->_x = this->_y = 1.0f;
	}

	void Vec2::Normalize()
	{
		float length = GetLength();
		if (length > 0)
		{
			this->_x /= length;
			this->_y /= length;
		}
	}

	Vec2 Vec2::GetNorlized() const
	{
		Vec2 result(*this);
		result.Normalize();
		return result;
	}

	float Vec2::Dot(const Vec2 & right)
	{
		return this->_x * right._x + this->_y * right._y;
	}

	

	Vec2 Vec2::operator*(float scale) const
	{
		return Vec2(this->_x * scale, this->_y * scale);
	}

	Vec2 Vec2::operator/(float scale) const
	{
		if (scale == 0)
			return *this;
		return Vec2(this->_x / scale, this->_y / scale);
	}

	Vec2 Vec2::operator+(float scale) const
	{
		return Vec2(this->_x + scale, this->_y + scale);
	}

	Vec2 Vec2::operator-(float scale) const
	{
		return Vec2(this->_x - scale, this->_y - scale);
	}

	Vec2 operator*(float scale, const Vec2 & right)
	{
		return right * scale;
	}

	Vec2 operator/(float scale, const Vec2 & right)
	{
		return right / scale;
	}

	Vec2 operator+(float scale, const Vec2 & right)
	{
		return right + scale;
	}

	Vec2 operator-(float scale, const Vec2 & right)
	{
		return right - scale;
	}

	Vec2 Vec2::operator+(const Vec2 & right) const
	{
		return Vec2(this->_x + right._x, this->_y + right._y);
	}

	Vec2 Vec2::operator-(const Vec2 & right) const
	{
		return Vec2(this->_x - right._x, this->_y - right._y);
	}

	Vec2 Vec2::operator*(const Vec2 & right) const
	{
		return Vec2(this->_x * right._x, this->_y * right._y);
	}

	

	Vec2 Vec2::operator*(const Matrix2 & right)
	{
		Vec2 result;
		float temp = 0.0f;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				temp += this->_data[j] * right._data[j * 2 + i];

			}
			result._data[i] = temp;
		}

		return result;
	}

	

	


	Vec3::Vec3()
	{
		this->_x = this->_y = this->_z = 0.0f;
	}

	Vec3::Vec3(float _x, float _y, float _z)
	{
		this->_x = _x;
		this->_y = _y;
		this->_z = _z;
	}

	void Vec3::Set(float _x, float _y, float _z)
	{
		this->_x = _x;
		this->_y = _y;
		this->_z = _z;
	}

	void Vec3::operator*=(const Matrix3 & right)
	{
		for (int i = 0; i < 3; i++)
		{
			float temp = 0.0f;
			for (int j = 0; j < 3; j++)
			{
				temp += this->_data[j] * right._data[j * 3 + i];

			}
			this->_data[i] = temp;
		}
	}

	Vec4::Vec4()
	{
		for(int index = 0; index < 4; index++)
			this->_data[index] = 0.0f;
	}

	Vec4::Vec4(float _x, float _y, float _z, float _w)
	{
		this->_x = _x;
		this->_y = _y;
		this->_z = _z;
		this->_w = _w;
	}

	Vec4 Vec4::operator*(const Matrix4 & right)
	{
		Vec4 result;
		for (int i = 0; i < 4; i++)
		{
			float temp = 0.0f;
			for (int j = 0; j < 4; j++)
			{
				temp += this->_data[j] * right._data[j * 4 + i];

			}
			result._data[i] = temp;
		}
		return result;
	}

	

}
