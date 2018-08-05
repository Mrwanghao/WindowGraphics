#include "Vector.h"
#include "Matrix.h"

namespace Math
{
	Vec2::Vec2()
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}

	Vec2::Vec2(float _x, float _y)
	{
		this->x = _x;
		this->y = _y;
	}

	Vec2::Vec2(const Vec2 & right)
	{
		this->x = right.x;
		this->y = right.y;
	}

	void Vec2::Set(float _x, float _y)
	{
		this->x = _x;
		this->y = _y;
	}

	void Vec2::SetX(float _x)
	{
		this->x = _x;
	}

	void Vec2::SetY(float _y)
	{
		this->y = _y;
	}

	float Vec2::GetX()
	{
		return this->x;
	}

	float Vec2::GetY()
	{
		return this->y;
	}

	void Vec2::LoadZero()
	{
		this->x = this->y = 0.0f;
	}

	void Vec2::LoadOne()
	{
		this->x = this->y = 1.0f;
	}

	void Vec2::Normalize()
	{
		float length = GetLength();
		if (length > 0)
		{
			this->x /= length;
			this->y /= length;
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
		return this->x * right.x + this->y * right.y;
	}

	Vec2 Vec2::operator*(float scale) const
	{
		return Vec2(this->x * scale, this->y * scale);
	}

	Vec2 Vec2::operator/(float scale) const
	{
		if (scale == 0)
			return *this;
		return Vec2(this->x / scale, this->y / scale);
	}

	Vec2 Vec2::operator+(float scale) const
	{
		return Vec2(this->x + scale, this->y + scale);
	}

	Vec2 Vec2::operator-(float scale) const
	{
		return Vec2(this->x - scale, this->y - scale);
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
		return Vec2(x + right.x, y + right.y);
	}

	Vec2 Vec2::operator-(const Vec2 & right) const
	{
		return Vec2(x - right.x, y - right.y);
	}

	Vec2 Vec2::operator*(const Vec2 & right) const
	{
		return Vec2(x * right.x, y * right.y);
	}

	void Vec2::operator+=(const Vec2 & right)
	{
		x += right.x;
		y += right.y;
	}

	Vec2 Vec2::operator*(const Matrix2 & right)
	{
		Vec2 result;
		for (int i = 0; i < 2; i++)
		{
			float temp = 0.0f;
			for (int j = 0; j < 2; j++)
			{
				temp += this->data[j] * right.data[j * 2 + i];
			}
			result.data[i] = temp;
		}

		return result;
	}

	Vec2 & Vec2::operator=(const Vec2 & right)
	{
		if(this == &right)
		{
			return *this;
		}

		x = right.x;
		y = right.y;
		return *this;
	}

	Vec3::Vec3()
	{
		this->x = this->y = this->z = 0.0f;
	}

	Vec3::Vec3(float _x, float _y, float _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	void Vec3::Set(float _x, float _y, float _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	void Vec3::Normalize()
	{
		float length = GetLength();
		if (length <= 0)
		{
			return;
		}

		this->x /= length;
		this->y /= length;
		this->z /= length;
	}

	float Vec3::GetLength() const
	{
		float length = sqrt(x*x + y*y + z*z);
		return length;
	}

	float Vec3::Dot(const Vec3 & right) const
	{
		return x * right.x + y * right.y + z * right.z;
	}

	void Vec3::operator*=(const Matrix3 & right)
	{
		for (int i = 0; i < 3; i++)
		{
			float temp = 0.0f;
			for (int j = 0; j < 3; j++)
			{
				temp += this->data[j] * right.data[j * 3 + i];

			}
			this->data[i] = temp;
		}
	}

	Vec3 Vec3::operator*(const Matrix4 & right) const
	{
		Vec4 result(this->x, this->y, this->z, 1.0f);
		result *= right;
		return Vec3(result.x, result.y, result.z);
	}

	Vec3 Vec3::operator-(const Vec3 & right) const
	{
		return Vec3(x - right.x, y - right.y, z - right.z);
	}

	Vec3 Vec3::operator*(float scale) const
	{
		return Vec3(x * scale, y * scale, z * scale);
	}

	Vec3 Vec3::operator+(const Vec3 & right) const
	{
		return Vec3(x + right.x, y + right.y, z + right.z);
	}

	void Vec3::operator+=(const Vec3 & right)
	{
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;
	}

	void Vec3::operator-=(const Vec3 & right)
	{
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
	}

	Vec3 Vec3::operator-()
	{
		Vec3 tmp;
		tmp.x = -x;
		tmp.y = -y;
		tmp.z = -z;
		return tmp;
	}

	void Vec3::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
	}

	Vec3& Vec3::operator=(const Vec3 & right)
	{
		if (this == &right)
		{
			return *this;
		}

		x = right.x;
		y = right.y;
		z = right.z;
		return *this;
	}

	Vec4::Vec4()
	{
		for(int index = 0; index < 4; index++)
			this->data[index] = 0.0f;
	}

	Vec4::Vec4(float _x, float _y, float _z, float _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	Vec4 Vec4::operator*(const Matrix4 & right)
	{
		Vec4 result;
		for (int i = 0; i < 4; i++)
		{
			float temp = 0.0f;
			for (int j = 0; j < 4; j++)
			{
				temp += this->data[j] * right.data[j * 4 + i];

			}
			result.data[i] = temp;
		}
		return result;
	}

	void Vec4::operator*=(const Matrix4 & right)
	{
		*this = *this * right;
	}

	Vec4& Vec4::operator=(const Vec4 & right)
	{
		if (this == &right)
		{
			return *this;
		}

		x = right.x;
		y = right.y;
		z = right.z;
		w = right.w;
		return *this;
	}

	Vec4 Vec4::operator*(const Vec4 & right)
	{
		return Vec4(x * right.x, y * right.y, z * right.z, w * right.w);
	}

	Vec4 Vec4::operator*(float scale)
	{
		return Vec4(x * scale, y * scale, z * scale, w * scale);
	}

	Vec4 Vec4::operator+(const Vec4 & right)
	{
		return Vec4(x + right.x, y + right.y, z + right.z, w + right.w);
	}

	Vec4 Vec4::operator-(const Vec4 & right)
	{
		return Vec4(x - right.x, y - right.y, z - right.z, w - right.w);
	}
	
}
