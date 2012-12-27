#include "glwindow.h"
#include "font.h"
#include <math.h>
#include <GL/glu.h>
#include <GL/glaux.h>

CGLWindow	window;
CFont		*font;

#define PI 3.1415926535
#define SIDES_NUM	100

void Initialize()
{
	font = new CFont();

	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,800,600,0);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
}

void Render()
{
	float alpha = abs(sinf(GetTickCount()/1000.0f));

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	font->SetColor(0.5,1,0.5,alpha);
	font->SetSize(2.0f);
	font->Print(180,270,"THIS IS MY FIRST TEXT!");
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
		default:
			break;
		}
		break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			window.Resize(LOWORD(lParam), HIWORD(lParam));
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	MSG msg;
	bool done = false;
	
	window.Initialize(800, 600, 32, false);
	
	Initialize();
	
	while(!done){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message==WM_QUIT){
				done = true;
			}
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			Render();
			window.Swap();
		}
	}
	
	window.Terminate();
	
	return msg.wParam;
}
