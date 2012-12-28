#include "glwindow.h"
#include <math.h>
#include <GL/glu.h>

CGLWindow window;

#define PI 3.1415926535
#define SIDES_NUM	100

void Initialize()
{
	GLfloat light_position[] = { 1.0f, 1.0f, 0.0f, 0.0f };
	GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	GLfloat material_diffuse[] = { 0.0f, 0.5f, 0.0f, 1.0f };
	GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat material_shininess[] = { 10.0f };
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.3f, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_CULL_FACE);
	
}

void Render()
{
	float alpha = GetTickCount()/10.0f;
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0,0,-2);
	glRotatef(alpha,1.0f,0.5f,0.3f);
	
	glBegin(GL_TRIANGLE_STRIP);
	for(int i=0;i<SIDES_NUM+1;i++){
		float x = 0.5f*sinf(i*2*PI/SIDES_NUM);
		float y = 0.5f*cosf(i*2*PI/SIDES_NUM);
		float d = sqrtf(x*x+y*y);
		glNormal3f(x/d,y/d,0);
		glVertex3f(x,y,-0.5f);
		glNormal3f(x/d,y/d,0);
		glVertex3f(x,y, 0.5f);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f,0.0f,-1.0f);
	glVertex3f(0.0f,0.0f,-0.5f);
	for(i=0;i<SIDES_NUM+1;i++){
		float x = 0.5f*sinf(i*2*PI/SIDES_NUM);
		float y = 0.5f*cosf(i*2*PI/SIDES_NUM);
		float d = sqrtf(x*x+y*y);
		glVertex3f(x,y,-0.5f);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f,0.0f, 1.0f);
	glVertex3f(0.0f,0.0f, 0.5f);
	for(i=0;i<SIDES_NUM+1;i++){
		float x = 0.5f*sinf(-i*2*PI/SIDES_NUM);
		float y = 0.5f*cosf(-i*2*PI/SIDES_NUM);
		float d = sqrtf(x*x+y*y);
		glVertex3f(x,y, 0.5f);
	}
	glEnd();
	
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
