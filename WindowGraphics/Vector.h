#pragma once
#include <math.h>

namespace Math 
{
	class Matrix2;
	class Matrix3;
	class Matrix4;

	class Vec2
	{
	public:
		Vec2();
		Vec2(float _x, float _y);
		Vec2(const Vec2& right);

	public:
		union 
		{
			float _data[2];
			struct 
			{
				float _x;
				float _y;
			};
		};

	public:
		void Set(float _x, float _y);
		void SetX(float _x);
		void SetY(float _y);
		float GetX();
		float GetY();

		void LoadZero();
		void LoadOne();

	public:
		inline float GetLength() const { return sqrt(_x * _x + _y * _y); }
		void Normalize();
		Vec2 GetNorlized() const;
		float Dot(const Vec2& right);
		

	public:
		Vec2 operator*(float scale) const;
		Vec2 operator/(float scale) const;
		Vec2 operator+(float scale) const;
		Vec2 operator-(float scale) const;
		friend Vec2 operator*(float scale, const Vec2& right);
		friend Vec2 operator/(float scale, const Vec2& right);
		friend Vec2 operator+(float scale, const Vec2& right);
		friend Vec2 operator-(float scale, const Vec2& right);

		Vec2 operator+(const Vec2& right) const;
		Vec2 operator-(const Vec2& right) const;
		Vec2 operator*(const Vec2& right) const;

		Vec2 operator*(const Matrix2& right);


		Vec2 operator-() const { return Vec2(-this->_x, -this->_y); }
		Vec2 operator+() const { return Vec2(this->_x, this->_y); }
	};

	class Vec3
	{
	public:
		Vec3();
		Vec3(float _x, float _y, float _z);

	public:
		union
		{
			float _data[3];
			struct
			{
				float _x;
				float _y;
				float _z;
			};
		};
	public:
		void Set(float _x, float _y, float _z);

	public:
		void operator*=(const Matrix3& right);

	};

	class Vec4
	{
	public:
		Vec4();
		Vec4(float _x, float _y, float _z, float _w);

	public:
		union
		{
			float _data[4];
			struct
			{
				float _x;
				float _y;
				float _z;
				float _w;
			};
		};
	public:
		Vec4 operator*(const Matrix4& right);

	};

	

}
