#pragma once

namespace Math
{
	class Vec4;

	class Matrix2
	{
	public:
		Matrix2();
		Matrix2(float _m00, float _m01, float _m10, float _m11);

	public:
		union
		{
			float _data[4];
			struct
			{
				float _m00, _m01;
				float _m10, _m11;
			};
		};

	public:
		void Transpose();
		float Determinant() const;

	public:
		Matrix2 operator*(float scale);
		Matrix2 operator*(const Matrix2& right);
	};

	class Matrix3
	{
	public:
		Matrix3();
		Matrix3(float _m00, float _m01, float _m02, float _m10, float _m11, float _m12, float _m20, float _m21, float _m22);

	public:
		union
		{
			float _data[9];
			struct
			{
				float _m00, _m01, _m02;
				float _m10, _m11, _m12;
				float _m20, _m21, _m22;
			};
		};
	};

	class Matrix4
	{
	public:
		Matrix4();
		Matrix4(float _m00, float _m01, float _m02, float _m03, float _m10, float _m11, float _m12, float _m13, float _m20, float _m21, float _m22, float _m23, float _m30, float _m31, float _m32, float _m33);

	public:
		union
		{
			float _data[16];
			struct
			{
				float _m00, _m01, _m02, _m03;
				float _m10, _m11, _m12, _m13;
				float _m20, _m21, _m22, _m23;
				float _m30, _m31, _m32, _m33;
			};
		};

	public:
		void Identity();
		void LoadTranslate(const Math::Vec4& translatePosition);
		void LoadRotX(float theta);
		void LoadRotY(float theta);
		void LoadRotZ(float theta);

	public:
		Matrix4 operator*(const Matrix4& right) const;
		void operator*=(const Matrix4& right);
		void operator=(const Matrix4& right);
	};

}
