#pragma once
#include <math.h>

class Quaternion
{
public:
	Quaternion();
	~Quaternion();

public:
	union
	{
		float _data[4];
		struct 
		{
			float w;
			float x;
			float y;
			float z;
		};
	};

public:
	inline float GetLength() const { return sqrt(w*w + x*x + y*y + z*z); }


};

