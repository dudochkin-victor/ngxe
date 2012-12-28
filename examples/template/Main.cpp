//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Project Template
//	Downloaded from: www.paulsprojects.net
//	Created:	12th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "GL files/glee.h"	//header for opengl versions >1.1 and extensions
#include <GL/glu.h>
#include "Maths/Maths.h"
#include "Log/LOG.h"
#include "Timer/TIMER.h"
#include "Fps Counter/FPS_COUNTER.h"
#include "Window/WINDOW.h"
#include "Bitmap Font/BITMAP_FONT.h"
#include "Image/IMAGE.h"
#include "Main.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

TIMER timer;
FPS_COUNTER fpsCounter;
BITMAP_FONT font;

COLOR backgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

//Set up GL
bool GLInit()
{
	//Init window
	if(!WINDOW::Instance()->Init(	"Project Template", 640, 480, 8, 8, 8, 8, 24, 8,
									false, false, true))
		return false;
	
	//Check for OpenGL version/extensions
	//if(!extgl_Extensions.OpenGL14)
	//{
		//LOG::Instance()->OutputError("I require OpenGL 1.4 support");
		//return false;
	//}

	//Init font
	if(!font.Init())
		return false;

	//set viewport
	int height=WINDOW::Instance()->height;
	if(height==0)
		height=1;

	glViewport(0, 0, WINDOW::Instance()->width, height);

	//Set up projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(	45.0f, (GLfloat)WINDOW::Instance()->width/(GLfloat)WINDOW::Instance()->height,
					1.0f, 100.0f);

	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	return true;
}

//Set up variables
bool DemoInit()
{
	//Demo initialisation here


	
	//reset timer
	timer.Reset();

	return true;
}

//Perform per-frame updates
void UpdateFrame()
{
	//set currentTime and timePassed
	static double lastTime=timer.GetTime();
	double currentTime=timer.GetTime();
	double timePassed=currentTime-lastTime;
	lastTime=currentTime;

	//Update window
	WINDOW::Instance()->Update();


	
	//Render frame
	RenderFrame(currentTime, timePassed);
}

//Render a frame
void RenderFrame(double currentTime, double timePassed)
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glTranslatef(0.0f, 0.0f,-5.0f);
	glRotatef((float)currentTime/10, 0.0f, 1.0f, 0.0f);

	//Create a simple rotating tetrahedron
	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3f(	 1.0f, 0.0f, 0.0f);
		glVertex3f(	 0.0f, 1.0f, 0.0f);

		glColor3f(	 0.0f, 1.0f, 0.0f);
		glVertex3f(	 1.0f,-1.0f, 1.0f);

		glColor3f(	 0.0f, 0.0f, 1.0f);
		glVertex3f(	 1.0f,-1.0f,-1.0f);

		glColor3f(	 0.0f, 1.0f, 0.0f);
		glVertex3f(	-1.0f,-1.0f,-1.0f);

		glColor3f(	 0.0f, 0.0f, 1.0f);
		glVertex3f(	-1.0f,-1.0f, 1.0f);

		glColor3f(	 0.0f, 1.0f, 0.0f);
		glVertex3f(	 1.0f,-1.0f, 1.0f);
	}
	glEnd();
	
	
	fpsCounter.Update();

	//Print text
	font.StartTextMode();
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	font.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());
	font.EndTextMode();

	WINDOW::Instance()->SwapBuffers();

	//Save a screenshot
	if(WINDOW::Instance()->IsKeyPressed(VK_F1))
	{
		WINDOW::Instance()->SaveScreenshot();
		WINDOW::Instance()->SetKeyReleased(VK_F1);
	}

	//Check for an openGL error
	WINDOW::Instance()->CheckGLError();

	//quit if necessary
	if(WINDOW::Instance()->IsKeyPressed(VK_ESCAPE))
		PostQuitMessage(0);
}

//Shut down demo
void DemoShutdown()
{
	font.Shutdown();
	WINDOW::Instance()->Shutdown();
}

//WinMain
int WINAPI WinMain(	HINSTANCE	hInstance,			//Instance
					HINSTANCE	hPrevInstance,		//Previous Instance
					LPSTR		lpCmdLine,			//Command line params
					int			nShowCmd)			//Window show state
{
	//Save hInstance
	WINDOW::Instance()->hInstance=hInstance;

	//Init GL and variables
	if(!GLInit())
	{
		LOG::Instance()->OutputError("OpenGL Initiation Failed");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("OpenGL Initiation Successful");

	if(!DemoInit())
	{
		LOG::Instance()->OutputError("Demo Initiation Failed");
		return false;
	}
	else
		LOG::Instance()->OutputSuccess("Demo Initiation Successful");

	//Main Loop
	for(;;)
	{
		if(!(WINDOW::Instance()->HandleMessages()))	//quit if HandleMessages returns false
			break;

		UpdateFrame();
	}

	//Shutdown
	DemoShutdown();

	//Exit program
	LOG::Instance()->OutputSuccess("Exiting...");
	return 0;
}
