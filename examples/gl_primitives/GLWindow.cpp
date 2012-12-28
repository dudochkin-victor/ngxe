#include "Global.h"
#include "GLWindow.h"

GLWindow::GLWindow(void)
{
	hWnd = NULL;
	hDC = NULL;
	hRC = NULL;
}

GLWindow::~GLWindow(void)
{
}

bool GLWindow::Initialize(int width, int height, bool fullscreen)
{
	WNDCLASS				wc;
	PIXELFORMATDESCRIPTOR	pfd;
	RECT					WindowRect;
	unsigned int			PixelFormat;
	DWORD					dwStyle = WS_OVERLAPPEDWINDOW;

	this->fullscreen = fullscreen;

	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_VREDRAW|CS_HREDRAW|CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "GLCLASS";

	RegisterClass(&wc);

	WindowRect.left=0;
	WindowRect.right=width;
	WindowRect.top=0;
	WindowRect.bottom=height;

	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, false);

	if(fullscreen){
		DEVMODE dm;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
		dm.dmSize	= sizeof(DEVMODE);
		dm.dmPelsWidth	= width;
		dm.dmPelsHeight = height;
		dm.dmBitsPerPel = 32;
		ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
		dwStyle = WS_POPUP;
	}

	hWnd = CreateWindow("GLCLASS", "GameProgrammer.org", 
		dwStyle|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		WindowRect.right-WindowRect.left, 
		WindowRect.bottom-WindowRect.top, 
		NULL, NULL, wc.hInstance, NULL);

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	hDC = GetDC(hWnd);

	if(!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(hWnd, "Failed to choose pixel format.", "ERROR", MB_OK);
		return false;
	}

	if(!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		MessageBox(hWnd, "Failed to set pixel format.", "ERROR", MB_OK);
		return false;
	}

	if(!(hRC = wglCreateContext(hDC)))
	{
		MessageBox(hWnd, "Failed to create the OpenGL rendering context.", "ERROR", MB_OK);
		return false;
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
		MessageBox(hWnd, "Failed to make current the OpenGL rendering context.", "ERROR", MB_OK);
		return false;
	}

	
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	
	glViewport(0, 0, width, height);

	return true;
}

void GLWindow::Terminate()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	if(fullscreen){
		ChangeDisplaySettings(NULL, 0);
	}
	DestroyWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}