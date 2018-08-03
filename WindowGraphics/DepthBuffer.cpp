#include "DepthBuffer.h"
#include <stdlib.h>
#include <string.h>

DepthBuffer::DepthBuffer(int _width, int _height)
	:
	width(_width),
	height(_height)
{
	buffer = (float*)malloc(sizeof(float) * width * height);
	memset(buffer, 0, sizeof(sizeof(float) * width * height));
}

DepthBuffer::~DepthBuffer()
{
	if (buffer)
	{
		free(buffer);
		buffer = nullptr;
	}
}

bool DepthBuffer::SetDepth(int row, int col, float invDepth)
{
	int jump = row * width + col;
	float prevInvDepth = buffer[jump];
	if (prevInvDepth >= invDepth)
	{
		return false;
	}

	buffer[jump] = invDepth;
	return true;
}
