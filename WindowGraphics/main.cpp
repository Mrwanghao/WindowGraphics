#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <ddraw.h>

#include "Vertex.h"
#include "Camera.h"
#include "Vector.h"
#include "GraphicsMod.h"
#include "MathUtil.h"
#include "Sampler.h"
#include "graphics.h"
#include "Triangle.h"

using namespace Math;

HWND main_window_hwnd		   = nullptr;
HINSTANCE main_window_instance = nullptr;

LPDIRECTDRAW7 lpdd = NULL;
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsprimary = NULL;
LPDIRECTDRAWSURFACE7 lpddsback = NULL;

extern Math::Matrix4 modelMatrix, viewMatrix, projectMatrix;

//Math::Vec4 temp1(-20.0f, 0.0f, 0.0f, 1.0f), temp2(50.0f, 50.0f, 0.0f, 1.0f), temp3(10.0f, 0.0f, 0.0f, 1.0f);
//Math::Vec4 temp1(-20.0f, 0.0f, 10.0f, 1.0f), temp2(50.0f, 50.0f, 20.0f, 1.0f), temp3(10.0f, 10.0f, 0.0f, 1.0f);

Vertex a = Vertex(Vec3(-2.0f, -2.0f, 2.0f), Vec3(0.0f, 0.0f, 0.0f), Math::Vec2(0.0f, 0.0f));
Vertex b = Vertex(Vec3(-2.0f,  2.0f, 2.0f), Vec3(0.0f, 0.0f, 0.0f), Math::Vec2(0.0f, 0.9f));
Vertex c = Vertex(Vec3( 2.0f,  2.0f, 2.0f), Vec3(0.0f, 0.0f, 0.0f), Math::Vec2(0.9f, 0.9f));
Vertex d = Vertex(Vec3( 2.0f, -2.0f, 2.0f), Vec3(0.0f, 0.0f, 0.0f), Math::Vec2(0.9f, 0.0f));

#if 1
Triangle tri(a, b, c);
Triangle triangle2(a, c, d);
#else
Triangle tri(a, b, d);
Triangle triangle2(b, c, d);

#endif
Camera mainCamera(Math::Vec3(0.0f, 0.0f, 0.0f), Math::Vec3(0.0f, 0.0f, -2.0f), 1.0f, 500.0f, 90.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

Math::Vec3 localPosition(0.0f, 0.0f, 0.0f);
Math::Vec3 localEular(0.0f, 1.0f, 0.0f);

int GameInit();
int GameMain();
int GameClose();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_CREATE:
		return 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int cmdShow)
{
	MSG msg = {0};

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS | CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	wcex.hIconSm = wcex.hIcon;
	
	main_window_instance = hInstance;

	if (!RegisterClassEx(&wcex))
		return -1;

	if (!(main_window_hwnd = CreateWindowEx(
											NULL, 
											WINDOW_CLASS_NAME, 
											"COMPUTER GRAPHICS", 
											WS_POPUP | WS_VISIBLE, 
											0,
											0,
											WINDOW_WIDTH,
											WINDOW_HEIGHT,
											NULL,
											NULL,
											main_window_instance,
											NULL)))
		return -1;

	GameInit();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GameMain();
	}

	GameClose();

	return msg.wParam;
}

int GameInit()
{
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
		return -1;

	if (FAILED(lpdd->SetCooperativeLevel(main_window_hwnd, DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
		return -1;

	//if (FAILED(lpdd->SetCooperativeLevel(main_window_hwnd, DDSCL_NORMAL)))
	//	return -1;

	if (FAILED(lpdd->SetDisplayMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, 0, 0)))
		return -1;

	memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.dwBackBufferCount = 1;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsprimary, NULL)))
		return -1;

	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsback)))
		return -1;
	
	return 0;
}



int GameMain()
{
	if (KEYDOWN(VK_ESCAPE))
		SendMessage(main_window_hwnd, WM_CLOSE, 0, 0);

	if (KEYDOWN(VK_F2))
		mainCamera.nearZ += 0.1f;

	if (KEYDOWN(VK_F1))
		mainCamera.nearZ += -0.1f;
	
	if (KEYDOWN(VK_LEFT))
		mainCamera.cameraDirection.y += 1.0f;

	if (KEYDOWN(VK_RIGHT))
		mainCamera.cameraDirection.y += -1.0f;

	if (KEYDOWN(VK_UP))
		mainCamera.worldPosition += Vec3(0.0f, 0.0f, 0.1f);
		
	if (KEYDOWN(VK_DOWN))
		mainCamera.worldPosition += Vec3(0.0f, 0.0f, -0.1f);

	if (KEYDOWN(VK_F3))
		mainCamera.worldPosition += Vec3(0.1f, 0.0f, 0.0f);

	if (KEYDOWN(VK_F4))
		mainCamera.worldPosition += Vec3(-0.1f, 0.0f, 0.0f);
	
	//localEular.y += 1.0f;
	//localEular.z += 1.0f;
	

	modelMatrix = GetModelMatrix(localPosition, localEular);
	mainCamera.CalWorldToCameraMatrix();
	//viewMatrix = GetViewMatrix(mainCamera.worldPosition, mainCamera.cameraDirection);
	projectMatrix = GetPerspectiveMatrix(90.0f, (float)((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT), mainCamera.nearZ, 500.0f);
	//
	//mainCamera.calWorldToCameraMatrix();
	//Math::Vec4 temp_01 = temp1, temp_02 = temp2, temp_03 = temp3;
	//
	//Math::Matrix4 localToWorldMatrix = eularToMatrix(localTOWorldRotation);
	////localTOWorldRotation.x += 1.0f;
	////localTOWorldRotation.y += 1.0f;
	////localTOWorldRotation.z += 1.0f;
	//
	//temp_01 = temp_01 * localToWorldMatrix;
	//temp_01 = temp_01 * mainCamera.worldToCameraMatrix;
	//mainCamera.cameraToPerspective(&temp_01);
	//mainCamera.perspectiveToScreen(&temp_01);
	//
	//temp_02 = temp_02 * localToWorldMatrix;
	//temp_02 = temp_02 * mainCamera.worldToCameraMatrix;
	//mainCamera.cameraToPerspective(&temp_02);
	//mainCamera.perspectiveToScreen(&temp_02);
	//
	//temp_03 = temp_03 * localToWorldMatrix;
	//temp_03 = temp_03 * mainCamera.worldToCameraMatrix;
	//mainCamera.cameraToPerspective(&temp_03);
	//mainCamera.perspectiveToScreen(&temp_03);


	if (FAILED(lpddsback->Lock(NULL, &ddsd,
		DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
		NULL)))
		return(0);

	DrawLineTriangle(tri, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	DrawLineTriangle(triangle2, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	
	//DrawClipLine(temp_01.x, temp_01.y, temp_02.x, temp_02.y, 0, rand() % 256, 0, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	//DrawClipLine(temp_01.x, temp_01.y, temp_03.x, temp_03.y, rand() % 256, 0, 0, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	//DrawClipLine(temp_02.x, temp_02.y, temp_03.x, temp_03.y, 0, 0, rand() % 256, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	DrawClipLine(0, 0, 320, 240, 0, rand() % 256, 0, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);

	//DrawTriangle((int)temp_01.x, (int)temp_01.y, (int)temp_02.x, (int)temp_02.y, (int)temp_03.x, (int)temp_03.y, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2, 0, rand() % 256, 0);
	//DrawGouraudTriangle((int)temp_01.x, (int)temp_01.y, (int)temp_02.x, (int)temp_02.y, (int)temp_03.x, (int)temp_03.y, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	// ²âÊÔÏß

	//DrawTextureTriangle((int)temp_01.x, (int)temp_01.y, (int)temp_02.x, (int)temp_02.y, (int)temp_03.x, (int)temp_03.y, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	


	if (FAILED(lpddsback->Unlock(NULL)))
		return(0);

	while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));

	//Sleep(33);

	DrawFillColor(lpddsback);
	
	return 0;
}



int GameClose()
{

	if (lpddsprimary)
	{
		lpddsprimary->Release();
		lpddsprimary = nullptr;
	}

	if (lpdd)
	{
		lpdd->Release();
		lpdd = nullptr;
	}

	return 0;
}


