#include "GraphicsMod.h"
#include "Vector.h"
#include "MathUtil.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Sampler.h"
#include "BMPTexture.h"


using namespace Math;

extern Matrix4 modelMatrix;

BMPTexture bmpTexture("test_bmp.bmp");
int nearZ = 1.0f;

#pragma region 绘制模块
void DrawLine(int x0, int y0, int x1, int y1, int red, int green, int blue, unsigned int *vb_start, int lpitch)
{
	int dx,             
		dy,             
		dx2,            
		dy2,
		x_inc,          
		y_inc,          
		error,          
		index;          

						
	vb_start = vb_start + x0 + y0 * lpitch;

	// compute horizontal and vertical deltas
	dx = x1 - x0;
	dy = y1 - y0;

	// test which direction the line is going in i.e. slope angle
	if (dx >= 0)
	{
		x_inc = 1;

	} // end if line is moving right
	else
	{
		x_inc = -1;
		dx = -dx;  // need absolute value

	} // end else moving left

	  // test y component of slope

	if (dy >= 0)
	{
		y_inc = lpitch;
	} // end if line is moving down
	else
	{
		y_inc = -lpitch;
		dy = -dy;  // need absolute value

	} // end else moving up

	  // compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;

	// now based on which delta is greater we can draw the line
	if (dx > dy)
	{
		// initialize error term
		error = dy2 - dx;
		//error = 0;

		// draw the line
		for (index = 0; index <= dx; index++)
		{
			// set the pixel
			*vb_start = RGB32BIT(0, red, green, blue);

			// test if error has overflowed
			if (error >= 0)
			{
				error -= dx2;

				// move to next line
				vb_start += y_inc;

			} // end if error overflowed

			  // adjust the error term
			error += dy2;

			// move to the next pixel
			vb_start += x_inc;

		} // end for

	} // end if |slope| <= 1
	else
	{
		// initialize error term
		error = dx2 - dy;
		//error = 0;

		// draw the line
		for (index = 0; index <= dy; index++)
		{
			// set the pixel
			*vb_start = RGB32BIT(0, red, green, blue);

			// test if error overflowed
			if (error >= 0)
			{
				error -= dy2;

				// move to next line
				vb_start += x_inc;

			} // end if error overflowed

			  // adjust the error term
			error += dx2;

			// move to the next pixel
			vb_start += y_inc;
		} // end for
	} // end else |slope| > 1
}

int  ClipLine(int & x1, int & y1, int & x2, int & y2)
{
	int clip_x1 = x1,
	    clip_y1 = y1,
	    clip_x2 = x2,
	    clip_y2 = y2;

	int p1Code = 0, p2Code = 0;

	if (x1 < MIN_CLIP_X)
	{
		p1Code |= LINE_CLIP_W;
	}
	else if (x1 > MAX_CLIP_X)
	{
		p1Code |= LINE_CLIP_E;
	}

	if (y1 < MIN_CLIP_Y)
	{
		p1Code |= LINE_CLIP_N;
	}
	else if (y1 > MAX_CLIP_Y)
	{
		p1Code |= LINE_CLIP_S;
	}

	if (x2 < MIN_CLIP_X)
	{
		p1Code |= LINE_CLIP_W;
	}
	else if (x2 > MAX_CLIP_X)
	{
		p1Code |= LINE_CLIP_E;
	}

	if (y2 < MIN_CLIP_Y)
	{
		p1Code |= LINE_CLIP_N;
	}
	else if (y2 > MAX_CLIP_Y)
	{
		p1Code |= LINE_CLIP_S;
	}

	if (p1Code & p2Code)
		return 0;

	if (p1Code == 0 & p1Code == 0)
		return 1;

	switch (p1Code)
	{
	case LINE_CLIP_C:
		break;
	case LINE_CLIP_E:
		clip_x1 = MAX_CLIP_X;
		
		clip_y1 = y1 + 0.5f + (MAX_CLIP_X - x1) * (y2 - y1) / (x2 - x1);
		break;

	case LINE_CLIP_S:
		clip_y1 = MAX_CLIP_Y;
		clip_x1 = x1 + 0.5f + (MAX_CLIP_Y - y1) * (x2 - x1) / (y2 - y1);
		break;

	case LINE_CLIP_W:
		clip_x1 = MIN_CLIP_X;
		clip_y1 = y1 + 0.5f + (MIN_CLIP_X - x1) * (y2 - y1) / (x2 - x1);
		break;

	case LINE_CLIP_N:
		clip_y1 = MIN_CLIP_Y;
		clip_x1 = x1 + 0.5f + (MIN_CLIP_Y - y1) * (x2 - x1) / (y2 - y1);
		break;

	case LINE_CLIP_ES:
		clip_y1 = MAX_CLIP_Y;
		clip_x1 = x1 + 0.5f + (MAX_CLIP_Y - y1) * (x2 - x1) / (y2 - y1);
		if (clip_x1 < MIN_CLIP_X || clip_x1 > MAX_CLIP_X)
		{
			clip_x1 = MAX_CLIP_X;
			clip_y1 = y1 + 0.5f + (MAX_CLIP_X - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	case LINE_CLIP_WS:
		clip_y1 = MAX_CLIP_Y;
		clip_x1 = x1 + 0.5f + (MAX_CLIP_Y - y1) * (x2 - x1) / (y2 - y1);
		if (clip_x1 < MIN_CLIP_X || clip_x1 > MAX_CLIP_X)
		{
			clip_x1 = MIN_CLIP_X;
			clip_y1 = y1 + 0.5f + (MIN_CLIP_X - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	case LINE_CLIP_EN:
		clip_y1 = MIN_CLIP_Y;
		clip_x1 = x1 + 0.5f + (MIN_CLIP_Y - y1) * (x2 - x1) / (y2 - y1);
		if (clip_x1 < MIN_CLIP_X || clip_x1 > MAX_CLIP_X)
		{
			clip_x1 = MAX_CLIP_X;
			clip_y1 = y1 + 0.5f + (MAX_CLIP_X - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	case LINE_CLIP_WN:
		clip_y1 = MIN_CLIP_Y;
		clip_x1 = x1 + 0.5f + (MIN_CLIP_Y - y1) * (x2 - x1) / (y2 - y1);
		if (clip_x1 < MIN_CLIP_X || clip_x1 > MAX_CLIP_X)
		{
			clip_x1 = MIN_CLIP_X;
			clip_y1 = y1 + 0.5f + (MIN_CLIP_X - x1) * (y2 - y1) / (x2 - x1);
		}
		break;
	}

	switch (p2Code)
	{
	case LINE_CLIP_C:
		break;
	case LINE_CLIP_E:
		clip_x2 = MAX_CLIP_X;
		clip_y2 = y2 + 0.5f + (MAX_CLIP_X - x2) * (y2 - y1) / (x2 - x1);
		break;

	case LINE_CLIP_S:
		clip_y2 = MAX_CLIP_Y;
		clip_x2 = x2 + 0.5f + (MAX_CLIP_Y - y2) * (x2 - x1) / (y2 - y1);
		break;

	case LINE_CLIP_W:
		clip_x2 = MIN_CLIP_X;
		clip_y2 = y2 + 0.5f + (MIN_CLIP_X - x2) * (y2 - y1) / (x2 - x1);
		break;

	case LINE_CLIP_N:
		clip_y2 = MIN_CLIP_Y;
		clip_x2 = x2 + 0.5f + (MIN_CLIP_Y - y2) * (x2 - x1) / (y2 - y1);
		break;

	case LINE_CLIP_ES:
		clip_y2 = MAX_CLIP_Y;
		clip_x2 = x2 + 0.5f + (MAX_CLIP_Y - y2) * (x2 - x1) / (y2 - y1);
		if (clip_x2 < MIN_CLIP_X || clip_x2 > MAX_CLIP_X)
		{
			clip_x2 = MAX_CLIP_X;
			clip_y2 = y2 + 0.5f + (MAX_CLIP_X - x2) * (y2 - y1) / (x2 - x1);
		}
		break;

	case LINE_CLIP_WS:
		clip_y2 = MAX_CLIP_Y;
		clip_x2 = x2 + 0.5f + (MAX_CLIP_Y - y2) * (x2 - x1) / (y2 - y1);
		if (clip_x2 < MIN_CLIP_X || clip_x2 > MAX_CLIP_X)
		{
			clip_x2 = MIN_CLIP_X;
			clip_y2 = y2 + 0.5f + (MIN_CLIP_X - x2) * (y2 - y1) / (x2 - x1);
		}
		break;

	case LINE_CLIP_EN:
		clip_y2 = MIN_CLIP_Y;
		clip_x2 = x2 + 0.5f + (MIN_CLIP_Y - y2) * (x2 - x1) / (y2 - y1);
		if (clip_x2 < MIN_CLIP_X || clip_x2 > MAX_CLIP_X)
		{
			clip_x2 = MAX_CLIP_X;
			clip_y2 = y2 + 0.5f + (MAX_CLIP_X - x2) * (y2 - y1) / (x2 - x1);
		}
		break;

	case LINE_CLIP_WN:
		clip_y2 = MIN_CLIP_Y;
		clip_x2 = x2 + 0.5f + (MIN_CLIP_Y - y2) * (x2 - x1) / (y2 - y1);
		if (clip_x2 < MIN_CLIP_X || clip_x2 > MAX_CLIP_X)
		{
			clip_x2 = MIN_CLIP_X;
			clip_y2 = y2 + 0.5f + (MIN_CLIP_X - x2) * (y2 - y1) / (x2 - x1);
		}
		break;
	}

	if (clip_x1 < MIN_CLIP_X || clip_x1 > MAX_CLIP_X || clip_y1 < MIN_CLIP_Y || clip_y1 > MAX_CLIP_Y ||
		clip_x2 < MIN_CLIP_X || clip_x2 > MAX_CLIP_X || clip_y2 < MIN_CLIP_Y || clip_y2 > MAX_CLIP_Y)
	{
		return 0;
	}

	x1 = clip_x1;
	x2 = clip_x2;
	y1 = clip_y1;
	y2 = clip_y2;

	return 1;
}

void DrawClipLine(float x1, float y1, float x2, float y2, int red, int green, int blue, unsigned int * video_buffer, unsigned int lPitch)
{
	int tempX1 = x1, tempY1 = y1, tempX2 = x2, tempY2 = y2;
	if (ClipLine(tempX1, tempY1, tempX2, tempY2))
	{
		DrawLine(tempX1, tempY1, tempX2, tempY2,red, green, blue, video_buffer, lPitch);
	}
}

void DrawFillColor(LPDIRECTDRAWSURFACE7 lpdds)
{
	DDBLTFX ddbltfx; 

	memset(&ddbltfx, 0, sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);

	ddbltfx.dwFillColor = 256;

	lpdds->Blt(NULL,      
		NULL,                 
		NULL,       
		DDBLT_COLORFILL | DDBLT_WAIT,                 
		&ddbltfx);  
}

void DrawBottomTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int* videoBuffer, int lPitch, int red, int green, int blue)
{
	float dx_left, dx_right, xs, xe, height;

	int tempX, tempY, right, left;

	// 为了保证 (x2, y2) 为左点 (x3, y3)为右点，这样便于理解
	if (x3 < x2)
	{
		tempX = x2;
		x2 = x3;
		x3 = tempX;
	}
	
	height = y3 - y1;

	dx_left = (x2 - x1) / height;
	dx_right = (x3 - x1) / height;

	//设置起点
	xs = (float)x1;
	xe = (float)x1;
	//xe = (float)x1 + 0.5f;

	if (y1 < MIN_CLIP_Y)
	{
		xs = xs + dx_left * (float)(MIN_CLIP_Y - y1);
		xe = xe + dx_right * (float)(MIN_CLIP_Y - y1);

		y1 = MIN_CLIP_Y;
	}

	if (y3 > MAX_CLIP_Y)
	{
		y3 = MAX_CLIP_Y;
	}

	//无论y1起点是哪  都可以到达相应的 行 缓存起点
	//unsigned int* destAddress = videoBuffer + y1 * lPitch;
	videoBuffer = videoBuffer +  y1 * lPitch;

	if (x1 >= MIN_CLIP_X && x1 <= MAX_CLIP_X &&
		x2 >= MIN_CLIP_X && x2 <= MAX_CLIP_X &&
		x3 >= MIN_CLIP_X && x3 <= MAX_CLIP_X)
	{
		//说明 整个三角形的三个顶点 都在矩形里面
		for (tempY = y1; tempY <= y3; tempY++, videoBuffer += lPitch)
		{
			// 是 xe - xs + 1的原因是 左上原则 
			//videoBuffer += (unsigned int)xs;
			for (int index = 0; index < xe - xs + 1; index++)
			{
				//videoBuffer += 1;
				/**(videoBuffer + (int)xs + index) = RGB32BIT(0, red, green, blue);*/
				*(videoBuffer + (int)xs + index) = RGB32BIT(0, rand() % 256, rand() % 256, rand() % 256);
			}

			//memset((unsigned int*)videoBuffer + (unsigned int)xs, rand() % 256, (xe - xs + 1) * sizeof(unsigned int));

			xs += dx_left;
			xe += dx_right;

		}
	}
	else
	{
		//循环每一行 去赋值
		for (tempY = y1; tempY <= y3; tempY++, videoBuffer += lPitch)
		{
			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			if (left < MIN_CLIP_X)
			{
				left = MIN_CLIP_X;
				if (right < MIN_CLIP_X)
				{
					continue;
				}

			}
			
			if (right > MAX_CLIP_X)
			{
				right = MAX_CLIP_X;
				if (left > MAX_CLIP_X)
				{
					continue;
				}
			
			}

			//videoBuffer += (unsigned int)left;
			for (int index = 0; index < right - left + 1; index++)
			{
				//videoBuffer += 1;
				//*(videoBuffer + left + index) = RGB32BIT(0, red, green, blue);
				*(videoBuffer + (int)left + index) = RGB32BIT(0, rand() % 256, rand() % 256, rand() % 256);
			}

			//memset((unsigned int*)videoBuffer + left, rand() % 256, (right - left + 1) * sizeof(unsigned int));

		}
	
	}


}

void DrawTopTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int * videoBuffer, int lPitch, int red, int green, int blue)
{
	float dx_right, dx_left, xs, xe, height;

	int tempX, tempY, right, left;

	//为了保证 (x1, y1) 为左点 (x2, y2) 为右点
	if (x2 < x1)
	{
		tempX = x2;
		x2 = x1;
		x1 = tempX;

	}

	height = y3 - y1;

	dx_left = (x3 - x1) / height;
	dx_right = (x3 - x2) / height;

	xs = (float)x1;
	xe = (float)(x2 + 0.5f);

	if (y1 < MIN_CLIP_Y)
	{
		xs = xs + dx_left * (float)(MIN_CLIP_Y - y1);
		xe = xe + dx_right * (float)(MIN_CLIP_Y - y1);

		y1 = MIN_CLIP_Y;
	}

	if (y3 > MAX_CLIP_Y)
	{
		y3 = MAX_CLIP_Y;
	}

	//unsigned int* destAddress = videoBuffer + lPitch * y1;
	videoBuffer += lPitch * y1;

	if (x1 >= MIN_CLIP_X && x1 <= MAX_CLIP_X &&
		x2 >= MIN_CLIP_X && x2 <= MAX_CLIP_X &&
		x3 >= MIN_CLIP_X && x3 <= MAX_CLIP_X)
	{
		for (tempY = y1; tempY <= y3; tempY++, videoBuffer += lPitch)
		{
			//videoBuffer += (unsigned int)xs;
			for (int index = 0; index < xe - xs + 1; index++)
			{
				//videoBuffer += 1;
				//*(videoBuffer + (int)xs + index)= RGB32BIT(0, red, green, blue);
				*(videoBuffer + (int)xs + index) = RGB32BIT(0, rand() * 256, rand() * 256, rand() * 256);
			}

			//memset((unsigned int*)videoBuffer+(unsigned int)xs, rand() % 256, (xe - xs + 1) * sizeof(unsigned int));

			xs += dx_left;
			xe += dx_right;
		}

	}
	else
	{
		for (tempY = y1; tempY <= y3; tempY++, videoBuffer += lPitch)
		{
			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			if (left < MIN_CLIP_X)
			{
				left = MIN_CLIP_X;
				if (right < MIN_CLIP_X)
				{	
					continue;
				}
			}

			if (right > MAX_CLIP_X)
			{
				right = MAX_CLIP_X;
				if (left > MAX_CLIP_X)
				{
					continue;
				}
			
			}

			//videoBuffer += (unsigned int)left;
			for (int index = 0; index < right - left + 1; index++)
			{
				//videoBuffer += 1;
				//*(videoBuffer + left + index)= RGB32BIT(0, red, green, blue);
				*(videoBuffer + (int)left + index) = RGB32BIT(0, rand() % 256, rand() % 256, rand() % 256);
			}

			//memset((unsigned int*)videoBuffer + left, rand() % 256, (right - left + 1) * sizeof(unsigned int));

		}

	}

}

void DrawTopGouraudTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int * videoBuffer, int lPitch)
{
	Vec3 bottomColor(1.0f, 0.0f, 0.0f), leftColor(0.0f, 1.0f, 0.0f), rightColor(0.0f, 0.0f, 1.0f);
	int left, right, tempY, tempX;

	float height = y3 - y1;

	//不需要交换y坐标  因为y1 = y2
	if (x1 > x2)
	{
		tempX = x1;
		x1 = x2;
		x2 = tempX;
	}

	float xs, xe;
	xs = x1;
	xe = x2;

	Vec3 tempLeftColor, tempRightColor;
	tempLeftColor = leftColor;
	tempRightColor = rightColor;

	Vec3 color_left, color_right;
	color_left = (bottomColor - leftColor) * (1 / height);
	color_right = (bottomColor - rightColor) * (1 / height);

	float dx_left = (x3 - x1) / height;
	float dx_right = (x3 - x2) / height;

	if (y1 < MIN_CLIP_Y)
	{
		xs = xs + (float)dx_left * (MIN_CLIP_Y - y1);
		xe = xe + (float)dx_right * (MIN_CLIP_Y - y1);

		tempLeftColor += color_left * (MIN_CLIP_Y - y1);
		tempRightColor += color_right * (MIN_CLIP_Y - y1);

		y1 = MIN_CLIP_Y;
	}

	if (y3 > MAX_CLIP_Y)
	{
		y3 = MAX_CLIP_Y;
	}

	videoBuffer += y1 * lPitch;

	Vec3 temp;
	if (x1 > MIN_CLIP_X && x1 < MAX_CLIP_X &&
		x2 > MIN_CLIP_X && x2 < MAX_CLIP_X &&
		x3 > MIN_CLIP_X && x3 < MAX_CLIP_X)
	{
		for (tempY = y1; tempY < y3; tempY++, videoBuffer += lPitch)
		{
			temp = tempLeftColor;

			Vec3 color_step = (tempRightColor - tempLeftColor) * (1 / (xe - xs  + 1));

			for (int index = 0; index < xe - xs + 1; index++)
			{
				*(videoBuffer + (int)xs + index) = RGB32BIT(0, (int)(temp.x*255), (int)(temp.y * 255), (int)(temp.z * 255));
				temp += color_step;

			}

			xs += dx_left;
			xe += dx_right;

			tempLeftColor += color_left;
			tempRightColor += color_right;
		}

	}
	else
	{
		for (int tempY = y1; tempY < y3; tempY++, videoBuffer += lPitch)
		{

			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			tempLeftColor += color_left;
			tempRightColor += color_right;

			Vec3 color_step = (tempRightColor - tempLeftColor) * (1 / (xe - xs + 1));
			temp = tempLeftColor;

			if (left < MIN_CLIP_X)
			{
				temp += color_step * (MIN_CLIP_X - left);

				left = MIN_CLIP_X;
				if (right < MIN_CLIP_X)
				{
					continue;
				}
			}

			if (right > MAX_CLIP_X)
			{
				right = MAX_CLIP_X;
				if (left > MAX_CLIP_X)
				{
					continue;
				}

			}

			for (int index = 0; index < right - left + 1; index++)
			{
				*(videoBuffer + left + index) = RGB32BIT(0, (int)(temp.x*255), (int)(temp.y * 255), (int)(temp.z * 255));
				temp += color_step;
			}
		}
	}
}

void DrawBottomGouraudTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int * videoBuffer, int lPitch)
{
	Vec3 topColor(1.0f, 0.0f, 0.0f), leftColor(0.0f, 1.0f, 0.0f), rightColor(0.0f, 0.0f, 1.0f);

	float height = y3 - y1;

	float xs, xe;
	xs = xe = x1;

	int left, right, tempY, tempX;

	Vec3 tempLeftColor, tempRightColor;
	tempLeftColor = tempRightColor = topColor;

	//不需要交换y坐标  因为y2 = y3
	if (x2 > x3)
	{
		tempX = x2;
		x2 = x3;
		x3 = tempX;
	}

	float dx_left = (x2 - x1) / height;
	float dx_right = (x3 - x1) / height;

	float inv_height = 1 / height;

	Vec3 color_left = (leftColor - topColor) * inv_height;
	Vec3 color_right = (rightColor - topColor) * inv_height;

	if (y1 < MIN_CLIP_Y)
	{
		xs = xs + (float)dx_left * (MIN_CLIP_Y - y1);
		xe = xe + (float)dx_right * (MIN_CLIP_Y - y1);

		tempLeftColor += color_left * (MIN_CLIP_Y - y1);
		tempRightColor += color_right * (MIN_CLIP_Y - y1);

		y1 = MIN_CLIP_Y;
	}

	if (y3 > MAX_CLIP_Y)
	{
		y3 = MAX_CLIP_Y;
	}

	videoBuffer += y1 * lPitch;

	Vec3 temp;
	if (x1 > MIN_CLIP_X && x1 < MAX_CLIP_X &&
		x2 > MIN_CLIP_X && x2 < MAX_CLIP_X &&
		x3 > MIN_CLIP_X && x3 < MAX_CLIP_X)
	{
		for (tempY = y1; tempY < y3; tempY++, videoBuffer += lPitch)
		{
			float inv = 1.0f / (xe - xs + 1);
			Vec3 color_step = (tempRightColor - tempLeftColor) * inv;

			temp = tempLeftColor;
			
			for (int index = 0; index < xe - xs + 1; index++)
			{
				*(videoBuffer + (int)xs + index) = RGB32BIT(0, (int)(temp.x * 255), (int)(temp.y * 255), (int)(temp.z * 255));
				temp = temp + color_step;

			}

			tempLeftColor  += color_left;
			tempRightColor += color_right;
			
			xs += dx_left;
			xe += dx_right;              
		}
	}
	else
	{
		for (int tempY = y1; tempY < y3; tempY++, videoBuffer += lPitch)
		{
			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			tempLeftColor += color_left;
			tempRightColor += color_right;

			Vec3 color_step = (tempRightColor - tempLeftColor) * (1 / (xe - xs + 1));
			temp = tempLeftColor;

			if (left < MIN_CLIP_X)
			{
				temp += color_step * (MIN_CLIP_X - left);

				left = MIN_CLIP_X;

				if (right < MIN_CLIP_X)
				{
					continue;
				}
			}

			if (right > MAX_CLIP_X)
			{
				right = MAX_CLIP_X;

				if (left > MAX_CLIP_X)
				{
					continue;
				}
			}

			for (int index = 0; index < right - left + 1; index++)
			{
				*(videoBuffer + left + index) = RGB32BIT(0, (int)(temp.x * 255), (int)(temp.y * 255), (int)(temp.z * 255));
				temp += color_step;
			}
		}
	}
}

void DrawGouraudTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int * videoBuffer, int lPitch)
{
	int tempX, tempY, newX;

	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
	{
		return;
	}

	if (y2 < y1)
	{
		tempX = x2;
		tempY = y2;

		x2 = x1;
		y2 = y1;

		x1 = tempX;
		y1 = tempY;
	}

	if (y3 < y1)
	{
		tempX = x3;
		tempY = y3;

		x3 = x1;
		y3 = y1;

		x1 = tempX;
		y1 = tempY;

	}

	if (y3 < y2)
	{
		tempX = x3;
		tempY = y3;

		x3 = x2;
		y3 = y2;

		x2 = tempX;
		y2 = tempY;

	}

	if (y3 < MIN_CLIP_Y ||
		y1 > MAX_CLIP_Y ||
		(x1 < MIN_CLIP_X && x2 < MIN_CLIP_X && x3 < MIN_CLIP_X) ||
		(x1 > MAX_CLIP_X && x2 > MAX_CLIP_X && x3 > MAX_CLIP_X))
	{
		return;
	}

	if (y1 == y2)
	{
		DrawTopGouraudTriangle(x1, y1, x2, y2, x3, y3, videoBuffer, lPitch);
	}
	else if (y2 == y3)
	{
		DrawBottomGouraudTriangle(x1, y1, x2, y2, x3, y3, videoBuffer, lPitch);
	}
	else
	{
		newX = x1 + (int)(0.5f + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));
		DrawBottomGouraudTriangle(x1, y1, newX, y2, x2, y2, videoBuffer, lPitch);
		DrawTopGouraudTriangle(x2, y2, newX, y2, x3, y3, videoBuffer, lPitch);
	}

}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int* videoBuffer, unsigned int lPitch, int red, int green, int blue)
{
	int tempX, tempY, newX;

	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
	{
		return;
	}

	if (y2 < y1)
	{
		tempX = x2;
		tempY = y2;

		x2 = x1;
		y2 = y1;

		x1 = tempX;
		y1 = tempY;
	}

	if (y3 < y1)
	{
		tempX = x3;
		tempY = y3;

		x3 = x1;
		y3 = y1;

		x1 = tempX;
		y1 = tempY;
	
	}

	if (y3 < y2)
	{
		tempX = x3;
		tempY = y3;

		x3 = x2;
		y3 = y2;

		x2 = tempX;
		y2 = tempY;
	
	}

	if (y3 < MIN_CLIP_Y || 
		y1 > MAX_CLIP_Y || 
		(x1 < MIN_CLIP_X && x2 < MIN_CLIP_X && x3 < MIN_CLIP_X) ||
		(x1 > MAX_CLIP_X && x2 > MAX_CLIP_X && x3 > MAX_CLIP_X))
	{
		return;
	}

	if (y1 == y2)
	{
		DrawTopTriangle(x1, y1, x2, y2, x3, y3, videoBuffer, lPitch, red, green, blue);
	}
	else if (y2 == y3)
	{
		DrawBottomTriangle(x1, y1, x2, y2, x3, y3, videoBuffer, lPitch, red, green, blue);
	}
	else
	{
		newX = x1 + (int)(0.5f + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));
		DrawBottomTriangle(x1, y1, newX, y2, x2, y2, videoBuffer, lPitch, red, green, blue);
		DrawTopTriangle(x2, y2, newX, y2, x3, y3, videoBuffer, lPitch, red, green, blue);
	}
}

void DrawTextureTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int *videoBuffer, int lPitch/*, Sampler sampler*/)
{
	int tempX, tempY, newX;

	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
	{
		return;
	}

	if (y2 < y1)
	{
		tempX = x1;
		tempY = y1;

		x1 = x2;
		y1 = y2;

		x2 = tempX;
		y2 = tempY;

	}

	if (y3 < y1)
	{
		tempX = x1;
		tempY = y1;

		x1 = x3;
		y1 = y3;

		x3 = tempX;
		y3 = tempY;
	
	}

	if (y3 < y2)
	{
		tempX = x2;
		tempY = y2;

		x2 = x3;
		y2 = y3;

		x3 = tempX;
		y3 = tempY;
	
	}

	if (y1 > MAX_CLIP_Y ||
		y3 < MIN_CLIP_Y ||
		(x1 < MIN_CLIP_X && x2 < MIN_CLIP_X && x3 < MIN_CLIP_X) ||
		(x1 > MAX_CLIP_X && x2 > MAX_CLIP_X && x3 > MAX_CLIP_X)
		)
	{
		return;
	}

	if (y2 == y3)
	{
		DrawBottomTextureTriangle(x1, y1, x2, y2, x3, y3, videoBuffer, lPitch/*, sampler*/);
	}
	else if (y1 == y2)
	{
		//DrawTopTextureTriangle(x1, y1, x2, y2, x3, y3, videoBuffer, lPitch, sampler);
	}
	else
	{
		newX = (int)(0.5f + x1 + (float)(y2 - y1) * (x3 - x1) / (y3 - y1));
		DrawBottomTextureTriangle(x1, y1, x2, y2, newX, y2, videoBuffer, lPitch/*, sampler*/);
		//DrawTopTextureTriangle(x2, y2, newX, y2, x3, y3, videoBuffer, lPitch, sampler);

	}

}

void DrawTopTextureTriangle(const Triangle& tri, unsigned int * videoBuffer, int lPitch/*, Sampler  sampler*/)
{
	

}

void DrawBottomTextureTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int * videoBuffer, int lPitch/*, Sampler  sampler*/)
{
	Vec2 topUV(0.0f, 0.0f), leftUV(1.0f, 0.0f), rightUV(0.0f, 1.0f);

	int tempX, tempY;

	if (x2 > x3)
	{
		tempX = x3;
		x3 = x2;
		x2 = tempX;
	}

	float height = y3 - y1;
	int left, right;

	float dx_left, dx_right;
	
	dx_left = (float)(x2 - x1) / height;
	dx_right = (float)(x3 - x1) / height;
	
	float xs = (float)x1;
	float xe = (float)x1;

	Vec2 tempLeftUV = topUV;
	Vec2 tempRightUV = topUV;

	Vec2 duv_left = (leftUV - topUV) / height;
	Vec2 duv_right = (rightUV - topUV) / height;

	if (y1 < MIN_CLIP_Y)
	{
		xs = (MIN_CLIP_Y - y1) * dx_left + xs;
		xe = (MIN_CLIP_Y - y1) * dx_right + xe;

		tempLeftUV += (MIN_CLIP_Y - y1) * duv_left;
		tempRightUV += (MIN_CLIP_Y - y1) * duv_right;

		y1 = MIN_CLIP_Y;
	}

	if (y3 > MAX_CLIP_Y)
	{
		y3 = MAX_CLIP_Y;
	}

	videoBuffer += y1 * lPitch;

	Vec2 tempUV;
	Vec4 tempColor;
	Vec2 uv_step;
	if (x1 > MIN_CLIP_X && x1 < MAX_CLIP_X &&
		x2 > MIN_CLIP_X && x2 < MAX_CLIP_X &&
		x3 > MIN_CLIP_X && x3 < MAX_CLIP_X)
	{
		for (tempY = y1; tempY < y3; tempY++, videoBuffer += lPitch)
		{
			tempUV = tempLeftUV;

			float inv = 1.0f / (xe - xs + 1);
			uv_step = (tempRightUV - tempLeftUV) * inv;

			for (int index = 0; index < xe - xs + 1; index++)
			{
				tempColor = bmpTexture.sampler->sample(tempUV.x, tempUV.y);
				*(videoBuffer + (int)xs + index) = RGB32BIT(0, (int)tempColor.x, (int)tempColor.y, (int)tempColor.z);
				
				tempUV += uv_step;
			}

			xs += dx_left;
			xe += dx_right;

			tempLeftUV += duv_left;
			tempRightUV += duv_right;
		}
	}
	else
	{
		for (tempY = y1; tempY < y3; tempY++, videoBuffer += lPitch)
		{
			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			tempLeftUV += duv_left;
			tempRightUV += duv_right;

			tempUV = tempLeftUV;
			uv_step = (tempRightUV - tempLeftUV) / (xe - xs + 1);

			if (left < MIN_CLIP_X)
			{
				tempUV += (MIN_CLIP_X - left) * uv_step;
				left = MIN_CLIP_X;
				if (right < MIN_CLIP_X)
				{
					continue;
				}
			}

			if (right > MAX_CLIP_X)
			{
				right = MAX_CLIP_X;
				if (left > MAX_CLIP_X)
				{
					continue;
				}
			}
			
			for (int index = 0; index < right - left + 1; index++)
			{
				tempColor = bmpTexture.sampler->sample(tempUV.x, tempUV.y);
				*(videoBuffer + left + index) = RGB32BIT(0, (int)tempColor.x, (int)tempColor.y, (int)tempColor.z);

				tempUV += uv_step;
			}
		}
	}
}

void DrawTextureTriangle(Triangle & tri, unsigned int * videoBuffer, int lPitch)
{
	tri.ndcA.x = (int)tri.ndcA.x;
	tri.ndcA.y = (int)tri.ndcA.y;
	tri.ndcB.x = (int)tri.ndcB.x;
	tri.ndcB.y = (int)tri.ndcB.y;
	tri.ndcC.x = (int)tri.ndcC.x;
	tri.ndcC.y = (int)tri.ndcC.y;

	Vec2 tempNDC;
	VertexOut tempVertexOut;

	if (tri.ndcA.y > tri.ndcB.y)
	{
		tempNDC = tri.ndcB;
		tempVertexOut = tri.vertexoutB;

		tri.ndcB = tri.ndcA;
		tri.vertexoutB = tri.vertexoutA;

		tri.ndcA = tempNDC;
		tri.vertexoutA = tempVertexOut;
	}

	if (tri.ndcA.y > tri.ndcC.y)
	{
		tempNDC = tri.ndcC;
		tempVertexOut = tri.vertexoutC;

		tri.ndcC = tri.ndcA;
		tri.vertexoutC = tri.vertexoutA;

		tri.ndcA = tempNDC;
		tri.vertexoutA = tempVertexOut;
	}

	if (tri.ndcB.y > tri.ndcC.y)
	{
		tempNDC = tri.ndcC;
		tempVertexOut = tri.vertexoutC;

		tri.ndcC = tri.ndcB;
		tri.vertexoutC = tri.vertexoutB;

		tri.ndcB = tempNDC;
		tri.vertexoutB = tempVertexOut;
	}
	
	if (tri.ndcA.y > MAX_CLIP_Y ||
		tri.ndcC.y < MIN_CLIP_Y ||
		(tri.ndcA.x < MIN_CLIP_X && tri.ndcB.x < MIN_CLIP_X && tri.ndcC.x < MIN_CLIP_X) ||
		(tri.ndcA.x > MAX_CLIP_X && tri.ndcB.x > MAX_CLIP_X && tri.ndcC.x > MAX_CLIP_X)
		)
	{
		return;
	}


	if ((int)tri.ndcB.y == (int)tri.ndcC.y)
	{
		DrawTextureBottomTriangle(tri, videoBuffer, lPitch);
	}
	else if ((int)tri.ndcA.y == (int)tri.ndcB.y)
	{
		DrawTextureTopTriangle(tri, videoBuffer, lPitch);
	}
	else
	{
		VertexOut tempVertexOut;
		Vec2 tempNDC;

		float param = (float)(tri.ndcB.y - tri.ndcA.y) / (tri.ndcC.y - tri.ndcA.y);
		tempNDC = clamp(tri.ndcA, tri.ndcC, param);
		//param = (float)(tri.vertexoutB.viewPosition.y - tri.vertexoutA.viewPosition.y) / (tri.vertexoutC.viewPosition.y - tri.vertexoutA.viewPosition.y);
		tempVertexOut = clamp(tri.vertexoutA, tri.vertexoutC, param);

		Triangle tempTri;
		tempTri.ndcA = tri.ndcA;
		tempTri.vertexoutA = tri.vertexoutA;
		tempTri.ndcB = tri.ndcB;
		tempTri.vertexoutB = tri.vertexoutB;
		tempTri.ndcC = tempNDC;
		tempTri.vertexoutC = tempVertexOut;
		DrawTextureBottomTriangle(tempTri, videoBuffer, lPitch);

		tempTri.ndcA = tri.ndcB;
		tempTri.vertexoutA = tri.vertexoutB;
		tempTri.ndcB = tempNDC;
		tempTri.vertexoutB = tempVertexOut;
		tempTri.ndcC = tri.ndcC;
		tempTri.vertexoutC = tri.vertexoutC;
		DrawTextureTopTriangle(tempTri, videoBuffer, lPitch);
		
	}
}

void DrawTextureTopTriangle(Triangle & tri, unsigned int * videoBuffer, int lPitch)
{
	Vec2 middleNDC;
	VertexOut tempVertexOut;

	if (tri.ndcA.x > tri.ndcB.x)
	{
		middleNDC = tri.ndcB;
		tempVertexOut = tri.vertexoutB;

		tri.ndcB = tri.ndcA;
		tri.vertexoutB = tri.vertexoutA;

		tri.ndcA = middleNDC;
		tri.vertexoutA = tempVertexOut;
	}

	float height = tri.ndcC.y - tri.ndcA.y;

	int left, right;

	float dx_left = (float)(tri.ndcC.x - tri.ndcA.x) / height;
	float dx_right = (float)(tri.ndcC.x - tri.ndcB.x) / height;

	float xs = (float)tri.ndcA.x;
	float xe = (float)tri.ndcB.x;

	Vec2 tempLeftUV = tri.vertexoutA.uv;
	Vec2 tempRightUV = tri.vertexoutB.uv;

	Vec2 duv_left = (tri.vertexoutC.uv - tri.vertexoutA.uv) / height;
	Vec2 duv_right = (tri.vertexoutC.uv - tri.vertexoutB.uv) / height;

	if (tri.ndcA.y < MIN_CLIP_Y)
	{
		xs = (MIN_CLIP_Y - tri.ndcA.y) * dx_left + xs;
		xe = (MIN_CLIP_Y - tri.ndcA.y) * dx_right + xe;

		tempLeftUV += (MIN_CLIP_Y - tri.ndcA.y) * duv_left;
		tempRightUV += (MIN_CLIP_Y - tri.ndcA.y) * duv_right;

		tri.ndcA.y = MIN_CLIP_Y;
	}

	if (tri.ndcC.y > MAX_CLIP_Y)
	{
		tri.ndcC.y = (float)MAX_CLIP_Y;
	}

	videoBuffer += (int)tri.ndcA.y * lPitch;

	Vec2 tempUV;
	Vec4 tempColor;
	Vec2 uv_step;

	int tempY;

	if (tri.ndcA.x > MIN_CLIP_X && tri.ndcA.x < MAX_CLIP_X &&
		tri.ndcB.x > MIN_CLIP_X && tri.ndcB.x < MAX_CLIP_X &&
		tri.ndcC.x > MIN_CLIP_X && tri.ndcC.x < MAX_CLIP_X)
	{
		for (tempY = tri.ndcA.y; tempY < tri.ndcC.y; tempY++, videoBuffer += lPitch)
		{
			tempUV = tempLeftUV;

			float inv = 1.0f / (xe - xs + 1);
			uv_step = (tempRightUV - tempLeftUV) * inv;

			for (int index = 0; index < xe - xs + 1; index++)
			{
				tempColor = bmpTexture.sampler->sample(tempUV.x, tempUV.y);
				*(videoBuffer + (int)xs + index) = RGB32BIT(0, (int)tempColor.x, (int)tempColor.y, (int)tempColor.z);

				tempUV += uv_step;
			}

			xs += dx_left;
			xe += dx_right;

			tempLeftUV += duv_left;
			tempRightUV += duv_right;
		}
	}
	else
	{
		for (tempY = tri.ndcA.y; tempY < tri.ndcC.y; tempY++, videoBuffer += lPitch)
		{
			left = (int)xs;
			right = (int)xe;

			tempUV = tempLeftUV;
			uv_step = (tempRightUV - tempLeftUV) / (right - left + 1);

			if (left < MIN_CLIP_X)
			{
				tempUV += (MIN_CLIP_X - left) * uv_step;
				left = MIN_CLIP_X;
				if (right < MIN_CLIP_X)
				{
					continue;
				}
			}

			if (right > MAX_CLIP_X)
			{
				right = MAX_CLIP_X;
				if (left > MAX_CLIP_X)
				{
					continue;
				}
			}

			for (int index = 0; index < right - left + 1; index++)
			{

				tempColor = bmpTexture.sampler->sample(tempUV.x, tempUV.y);

				*(videoBuffer + left + index) = RGB32BIT(0, (int)tempColor.x, (int)tempColor.y, (int)tempColor.z);
				
				tempUV += uv_step;
				
			}

			xs += dx_left;
			xe += dx_right;

			tempLeftUV += duv_left;
			tempRightUV += duv_right;
		}
	}
}

void DrawTextureBottomTriangle(Triangle & tri, unsigned int * videoBuffer, int lPitch)
{
	Vec2 middleNDC;
	VertexOut tempVertexOut;

	if (tri.ndcB.x > tri.ndcC.x)
	{
		middleNDC = tri.ndcC;
		tempVertexOut = tri.vertexoutC;

		tri.ndcC = tri.ndcB;
		tri.vertexoutC = tri.vertexoutB;

		tri.ndcB = middleNDC;
		tri.vertexoutB = tempVertexOut;
	}

	float height = tri.ndcC.y - tri.ndcA.y;

	int left, right;
	float dx_left, dx_right;

	dx_left = (float)(tri.ndcB.x - tri.ndcA.x) / height;
	dx_right = (float)(tri.ndcC.x - tri.ndcA.x) / height;

	float xs = (float)tri.ndcA.x;
	float xe = (float)tri.ndcA.x;

	Vec2 tempLeftUV = tri.vertexoutA.uv;
	Vec2 tempRightUV = tri.vertexoutA.uv;

	Vec2 duv_left = (tri.vertexoutB.uv - tri.vertexoutA.uv) / height;
	Vec2 duv_right = (tri.vertexoutC.uv - tri.vertexoutA.uv) / height;
	
	if (tri.ndcA.y < MIN_CLIP_Y)
	{
		xs = (MIN_CLIP_Y - tri.ndcA.y) * dx_left + xs;
		xe = (MIN_CLIP_Y - tri.ndcA.y) * dx_right + xe;

		tempLeftUV += (MIN_CLIP_Y - tri.ndcA.y) * duv_left;
		tempRightUV += (MIN_CLIP_Y - tri.ndcA.y) * duv_right;

		tri.ndcA.y = MIN_CLIP_Y;
	}

	if (tri.ndcC.y > MAX_CLIP_Y)
	{
		tri.ndcC.y = MAX_CLIP_Y;
	}

	videoBuffer += (int)tri.ndcA.y * lPitch;

	Vec2 tempUV;
	Vec4 tempColor;
	Vec2 uv_step;

	int tempY;
	
	if (tri.ndcA.x > MIN_CLIP_X && tri.ndcA.x < MAX_CLIP_X &&
		tri.ndcB.x > MIN_CLIP_X && tri.ndcB.x < MAX_CLIP_X &&
		tri.ndcC.x > MIN_CLIP_X && tri.ndcC.x < MAX_CLIP_X)
	{
		for (tempY = tri.ndcA.y; tempY < tri.ndcC.y; tempY++, videoBuffer += lPitch)
		{
			tempUV = tempLeftUV;

			float inv = 1.0f / (xe - xs + 1);
			uv_step = (tempRightUV - tempLeftUV) * inv;

			for (int index = 0; index < xe - xs + 1; index++)
			{
				tempColor = bmpTexture.sampler->sample(tempUV.x, tempUV.y);
				*(videoBuffer + (int)xs + index) = RGB32BIT(0, (int)tempColor.x, (int)tempColor.y, (int)tempColor.z);

				tempUV += uv_step;
			}

			xs += dx_left;
			xe += dx_right;

			tempLeftUV += duv_left;
			tempRightUV += duv_right;
		}
	}
	else
	{
		for (tempY = tri.ndcA.y; tempY < tri.ndcC.y; tempY++, videoBuffer += lPitch)
		{
			left = (int)xs;
			right = (int)xe;

			xs += dx_left;
			xe += dx_right;

			tempLeftUV += duv_left;
			tempRightUV += duv_right;

			tempUV = tempLeftUV;
			uv_step = (tempRightUV - tempLeftUV) / (right - left + 1);

			if (left < MIN_CLIP_X)
			{
				tempUV += (MIN_CLIP_X - left) * uv_step;
				left = MIN_CLIP_X;
				if (right < MIN_CLIP_X)
				{
					continue;
				}
			}

			if (right > MAX_CLIP_X)
			{
				right = MAX_CLIP_X;
				if (left > MAX_CLIP_X)
				{
					continue;
				}
			}

			for (int index = 0; index < right - left + 1; index++)
			{
				tempColor = bmpTexture.sampler->sample(tempUV.x, tempUV.y);
				*(videoBuffer + left + index) = RGB32BIT(0, (int)tempColor.x, (int)tempColor.y, (int)tempColor.z);

				tempUV += uv_step;
			}
		}
	}
}

#pragma endregion
