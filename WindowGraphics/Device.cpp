#include "Device.h"
#include "FrameBuffer.h"
#include "DepthBuffer.h"

Device::Device(int _width, int _height)
	:
	width(_width),
	height(_height)
{
	depth = new DepthBuffer(width, height);
	frame = new FrameBuffer(width, height);
}

Device::~Device()
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

void Device::SetPixel(int row, int col, float invDepth, Color color)
{
	if (depth->SetDepth(row, col, invDepth))
	{
		frame->SetPixel(row, col, color);
	}
}

//用于提交像素图
void Device::Flush()
{

}
