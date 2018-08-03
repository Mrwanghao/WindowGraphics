#include "FrameBuffer.h"
#include <stdlib.h>
#include <string.h>

FrameBuffer::FrameBuffer(int _width, int _height)
	:
	width(_width),
	height(_height)
{
	buffer = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 3);
	memset(buffer, 0, sizeof(unsigned char) * width * height * 3);
}

FrameBuffer::~FrameBuffer()
{
	if (buffer)
	{
		free(buffer);
		buffer = nullptr;
	}
}

void FrameBuffer::SetPixel(int row, int col, Color color)
{
	if (!buffer)
	{
		return;
	}
	color *= 255;
	int jump = (row * width + col) * 3;
	buffer[jump + 0] = (unsigned char)color.x;
	buffer[jump + 1] = (unsigned char)color.y;
	buffer[jump + 2] = (unsigned char)color.z;
}
