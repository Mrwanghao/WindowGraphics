#pragma once

#define WINDOW_CLASS_NAME "WINDOW GRAPHICS"
#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480
#define WINDOW_BPP		32

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0))
#define KEYUP(vk_code)	 ((GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1))

#define RGB32BIT(a, r, g, b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

#define LINE_CLIP_S  0x0001
#define LINE_CLIP_E  0x0002
#define LINE_CLIP_W  0x0004
#define LINE_CLIP_N  0x0008

#define LINE_CLIP_ES 0x0003
#define LINE_CLIP_WS 0x0005
#define LINE_CLIP_EN 0x000a
#define LINE_CLIP_WN 0x000c

#define MIN_CLIP_X 0
#define MIN_CLIP_Y 0
#define MAX_CLIP_X (WINDOW_WIDTH  - 1)
#define MAX_CLIP_Y (WINDOW_HEIGHT - 1)

void DrawLine(int x0, int y0, int x1, int y1, int red, int green, int blue, unsigned int *vb_start, int lpitch);
void ClipLine(int &x1, int &y1, int &x2, int &y2);