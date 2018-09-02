#include "Sampler.h"
#include <memory.h>
#include <stdlib.h>
#include <iostream>

Sampler::Sampler(int samplerWidth, int samplerHeight)
	:
	width(samplerWidth),
	height(samplerHeight)
{
	imageData = new unsigned char[width * height * 3];
	memset(imageData, 0, width * height * 3 * sizeof(unsigned char));
}

Sampler::~Sampler()
{
	if (imageData)
	{
		delete[] imageData;
		imageData = nullptr;
	}
}

/******************
* 00 01
* 10 11
* 看这个采样点里谁更近 那么就影响越多
*****************/

Vec4 Sampler::sample(float s, float t)
{
	if (s < 0.0f || s > 1.0f || t < 0.0f || t > 1.0f)
		return Vec4(0, 0, 255, 255);
#if 1
	int allLength = width * height * 3;
	
	Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
	float u = (float)(this->width - 1) * s;
	float v = (float)(this->height - 1) * (1.0f - t); // windows  倒放
	
	int iu = (int)u;
	int iv = (int)v;
	
	int uNext = (iu + 1) <= (width - 1) ? iu + 1 : (width - 1);
	int vNext = (iv + 1) <= (height -1) ? iv + 1 : (height - 1);
	
	float uNextPer = u - iu;
	float vNextPer = v - iv;
	
	float uPer = 1.0f - uNextPer;
	float vPer = 1.0f - vNextPer;
	
	unsigned int imageIndex = (iv * width + iu) * 3;
	
	color.x = (float)imageData[imageIndex + 0];
	color.y = (float)imageData[imageIndex + 1];
	color.z = (float)imageData[imageIndex + 2];
	
	unsigned int imageIndexNextU  = (width * iv    + uNext) * 3;
	unsigned int imageIndexNextV  = (width * vNext + iu   ) * 3;
	unsigned int imageIndexNextUV = (width * vNext + uNext) * 3;
	
	Vec4 colorNextU(1.0f, 1.0f, 1.0f, 1.0f), colorNextV(1.0f, 1.0f, 1.0f, 1.0f), colorNextUV(1.0f, 1.0f, 1.0f, 1.0f);
	
	colorNextU.x  = (float)imageData[imageIndexNextU];
	colorNextU.y  = (float)imageData[imageIndexNextU];
	colorNextU.z  = (float)imageData[imageIndexNextU];
		  									
	colorNextV.x = (float)imageData[imageIndexNextV];
	colorNextV.y = (float)imageData[imageIndexNextV];
	colorNextV.z = (float)imageData[imageIndexNextV];
											 
	colorNextUV.x = (float)imageData[imageIndexNextUV];
	colorNextUV.y = (float)imageData[imageIndexNextUV];
	colorNextUV.z = (float)imageData[imageIndexNextUV];
	
	color.x = 
		color.x * uPer * vPer + 
		colorNextU.x * uNextPer * vPer + 
		colorNextV.x * vNextPer * uPer + 
		colorNextUV.x * uNextPer * vNextPer;
	
	color.y = 
		color.y * uPer * vPer + 
		colorNextU.y * uNextPer * vPer + 
		colorNextV.y * vNextPer * uPer+ 
		colorNextUV.y * uNextPer * vNextPer;
	
	color.z = 
		color.z * uPer * vPer + 
		colorNextU.z * uNextPer *vPer + 
		colorNextV.z * vNextPer * uPer + 
		colorNextUV.z * uNextPer *vNextPer;
	
	return color;
#else 
	return Vec4(1.0f, 0.0f, 0.0f, 1.0f);
#endif
}
