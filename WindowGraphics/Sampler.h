#pragma once
#include "Vector.h"
#include "BMPFile.h"

class Sampler
{
private:
	int width;
	int height;

public:
	unsigned char* imageData;
	
	Sampler(int samplerWidth, int samplerHeight);
	~Sampler();
	Math::Vec4 sample(float s, float t);

};