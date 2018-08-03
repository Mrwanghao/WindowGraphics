#pragma once
#include "DataType.h"

class FrameBuffer
{
	int width;
	int height;
	unsigned char* buffer;

public:
	FrameBuffer(int _width, int _height);
	~FrameBuffer();

public:
	void SetPixel(int row, int col, Color color);
};
