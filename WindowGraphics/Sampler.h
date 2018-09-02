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
	Vec4 sample(float s, float t);

};