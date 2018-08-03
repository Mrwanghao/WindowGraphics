#pragma once

// use 1/z

class DepthBuffer
{
	int width;
	int height;
	float* buffer;

public:
	DepthBuffer(int _width, int _height);
	~DepthBuffer();

public:
	bool SetDepth(int row, int col, float invDepth);
};