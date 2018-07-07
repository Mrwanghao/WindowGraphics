#include "GraphicsMod.h"


void DrawLine(int x0, int y0, int x1, int y1, int red, int green, int blue, unsigned int *vb_start, int lpitch)
{
	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2,
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping

						// pre-compute first pixel address in video buffer
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

void ClipLine(int & x1, int & y1, int & x2, int & y2)
{
	float clip_x1 = x1,
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
		p1Code |= LINE_CLIP_S;
	}
	else if (y1 > MAX_CLIP_Y)
	{
		p1Code |= LINE_CLIP_N;
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
		p1Code |= LINE_CLIP_S;
	}
	else if (y2 > MAX_CLIP_Y)
	{
		p1Code |= LINE_CLIP_N;
	}

	float m = (x2 - x1) / (y2 - y1);

	switch (p1Code)
	{
	case LINE_CLIP_E:
		clip_x1 = MAX_CLIP_X;
		clip_y1 = x1 + 0.5f + (clip_x1 - x1) / m;
		break;

	case LINE_CLIP_S:
		clip_y1 = MAX_CLIP_Y;
		clip_x1 = x1 + 0.5f + (clip_y1 - y1) * m;
		break;

	case LINE_CLIP_W:
		clip_x1 = MIN_CLIP_X;
		clip_y1 = x1 + 0.5f + (clip_x1 - x1) / m;
		break;

	case LINE_CLIP_N:
		clip_y1 = MIN_CLIP_Y;
		clip_x1 = x1 + 0.5f + (clip_y1 - y1) * m;
		break;

	case LINE_CLIP_ES:

		break;

	case LINE_CLIP_WS:

		break;

	case LINE_CLIP_EN:

		break;

	case LINE_CLIP_WN:

		break;
	}


}
