#pragma once

#include "DataType.h"
#include "Camera.h"

class DepthBuffer;
class FrameBuffer;



class Context
{
	DepthBuffer* depth;
	FrameBuffer* frame;
	int width;
	int height;

public:
	Context(int _width, int _height);
	~Context();

public:
	void SetPixel(int row, int col, float invDepth, Color color);
	void Flush();

};