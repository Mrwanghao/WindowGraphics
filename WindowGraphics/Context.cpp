#include "Context.h"
#include "FrameBuffer.h"
#include "DepthBuffer.h"

extern unsigned int* colorBuffer;

Context::Context(int _width, int _height)
	:
	width(_width),
	height(_height)
{
	depth = new DepthBuffer(width, height);
	frame = new FrameBuffer(width, height);
}

Context::~Context()
{
	if (depth)
	{
		delete depth;
		depth = nullptr;
	}

	if (frame)
	{
		delete frame;
		frame = nullptr;
	}

}

void Context::SetPixel(int row, int col, float invDepth, Color color)
{
	if (depth->SetDepth(row, col, invDepth))
	{
		frame->SetPixel(row, col, color);
	}
}

//用于提交像素图
void Context::Flush()
{

}
