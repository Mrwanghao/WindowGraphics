#pragma once

#include "DataType.h"

class DepthBuffer;
class FrameBuffer;


class Device
{
	DepthBuffer* depth;
	FrameBuffer* frame;

	int width;
	int height;

public:
	Device(int _width, int _height);
	~Device();

public:
	void SetPixel(int row, int col, float invDepth, Color color);
	void Flush();

};