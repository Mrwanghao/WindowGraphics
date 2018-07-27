#pragma once

class Sampler;

class BMPTexture
{
private:

public:
	BMPTexture(const char *fileName);
	~BMPTexture();

	Sampler * sampler;
};