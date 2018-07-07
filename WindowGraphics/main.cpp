#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include "Camera.h"

#include <ddraw.h>

#include "Vector.h"
#include "GraphicsMod.h"
#include "MathUtil.h"

using namespace Math;



HWND main_window_hwnd = nullptr;
HINSTANCE main_window_instance = nullptr;

LPDIRECTDRAW7 lpdd = NULL;
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpddsprimary = NULL;
LPDIRECTDRAWSURFACE7 lpddsback = NULL;

//Math::Vec4 temp1(-20.0f, 0.0f, 0.0f, 1.0f), temp2(50.0f, 50.0f, 0.0f, 1.0f), temp3(10.0f, 0.0f, 0.0f, 1.0f);
Math::Vec4 temp1(-20.0f, 0.0f, 10.0f, 1.0f), temp2(50.0f, 50.0f, 20.0f, 1.0f), temp3(10.0f, 0.0f, 0.0f, 1.0f);

Camera mainCamera(Math::Vec4(0.0f, 90.0f, 0.0f, 1.0f), Math::Vec4(0.0f, 0.0f, -100.0f, 1.0f), 50.0f, 500.0f, 90.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

Math::Vec3 localTOWorldRotation(0.0f, 0.0f, 0.0f);

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
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
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
	

	mainCamera.calWorldToCameraMatrix();
	return 0;
}


int GameMain()
{
	if (KEYDOWN(VK_ESCAPE))
		SendMessage(main_window_hwnd, WM_CLOSE, 0, 0);

	//mainCamera.cameraDirection._y += 10.0f;

	Math::Vec4 temp_01 = temp1, temp_02 = temp2, temp_03 = temp3;

	Math::Matrix4 localToWorldMatrix = eularToMatrix(localTOWorldRotation);
	localTOWorldRotation._y += 10.0f;

	temp_01 = temp_01 * localToWorldMatrix;
	temp_01 = temp_01 * mainCamera.worldToCameraMatrix;
	mainCamera.cameraToPerspective(&temp_01);
	mainCamera.perspectiveToScreen(&temp_01);

	temp_02 = temp_02 * localToWorldMatrix;
	temp_02 = temp_02 * mainCamera.worldToCameraMatrix;
	mainCamera.cameraToPerspective(&temp_02);
	mainCamera.perspectiveToScreen(&temp_02);

	temp_03 = temp_03 * localToWorldMatrix;
	temp_03 = temp_03 * mainCamera.worldToCameraMatrix;
	mainCamera.cameraToPerspective(&temp_03);
	mainCamera.perspectiveToScreen(&temp_03);

	if (FAILED(lpddsback->Lock(NULL, &ddsd,
		DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
		NULL)))
		return(0);

	DrawLine(temp_01._x, temp_01._y, temp_02._x, temp_02._y, 0, 255, 0, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	DrawLine(temp_01._x, temp_01._y, temp_03._x, temp_03._y, 255, 0, 0, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	DrawLine(temp_02._x, temp_02._y, temp_03._x, temp_03._y, 0, 0, 255, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);

	DrawLine(50.0f, 10.0f, 150.0f, 250.0f, 0, 255, 0, (UINT*)ddsd.lpSurface, ddsd.lPitch >> 2);
	
	/*DrawLine(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT,
		rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT,
		rand() % 256, rand() % 256, rand() % 256,
		(UINT *)ddsd.lpSurface, ddsd.lPitch >> 2);*/

	// unlock primary buffer
	if (FAILED(lpddsback->Unlock(NULL)))
		return(0);

	while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));

	Sleep(3000);

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


