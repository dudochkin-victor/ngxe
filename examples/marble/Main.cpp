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
#include <stdlib.h>
#include <time.h>
#include "GL files/glee.h"	//header for opengl versions >1.1 and extensions
#include <GL/glu.h>
#include "Maths/Maths.h"
#include "Log/LOG.h"
#include "Timer/TIMER.h"
#include "Fps Counter/FPS_COUNTER.h"
#include "Window/WINDOW.h"
#include "Bitmap Font/BITMAP_FONT.h"
#include "Image/IMAGE.h"
#include "GL files/LoadProgram.h"
#include "Teapot.h"
#include "Main.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

TIMER timer;
FPS_COUNTER fpsCounter;
BITMAP_FONT font;

COLOR backgroundColor(0.0f, 0.0f, 0.4f, 0.0f);

//Marble vertex/fragment programs
GLuint marbleVP;
GLuint marbleFP;

//Noise gradient texture
GLuint gradientTexture;

//weight factor lookup texture (x, y, z) => (1-(3x^2 - 2x^3))*(1-(3y^2 - 2y^3))*(1-(3z^2 - 2z^3))
GLuint weightTexture;

//Color lookup table
GLuint colorTexture;

//Set up GL
bool GLInit()
{
	//Init window
	if(!WINDOW::Instance()->Init(	"Project Template", 640, 480, 8, 8, 8, 8, 24, 8,
									false, false, true))
		return false;
	
	//Check for OpenGL version 1.3 & extensions
	if(	!GLEE_VERSION_1_3 || !GLEE_ARB_fragment_program ||
		!GLEE_ARB_vertex_program)
	{
		LOG::Instance()->OutputError("I require OpenGL 1.3, ARB_fragment_program and");
		LOG::Instance()->OutputError("ARB_vertex_program support");
		
		return false;
	}

	//Check for 2 texture image units
	GLint maxTexImageUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxTexImageUnits);
	if(maxTexImageUnits<2)
	{
		LOG::Instance()->OutputError("I require at least 2 texture image units");
		return false;
	}

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

	return true;
}

//Set up variables
bool DemoInit()
{
	//Create and load the vertex/fragment programs
	glGenProgramsARB(1, &marbleVP);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, marbleVP);
	if(!LoadProgram(GL_VERTEX_PROGRAM_ARB, "marbleVP.txt"))
		return false;

	glGenProgramsARB(1, &marbleFP);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, marbleFP);
	if(!LoadProgram(GL_FRAGMENT_PROGRAM_ARB, "marbleFP.txt"))
		return false;

	//Seed random number generator
	srand((unsigned int)time(NULL));

	//Generate 64x64x64 3d texture of gradients for noise
	
	//Create temporary space to hold gradient values
	GLubyte * gradientValues=new GLubyte[64*64*64*3];
	if(!gradientValues)
	{
		LOG::Instance()->OutputError("Unable to create space for gradient values");
		return false;
	}

	for(int i=0; i<64*64*64; ++i)
	{
		double phi=(2.0*M_PI*rand())/RAND_MAX;
		double theta=(M_PI*rand())/RAND_MAX;

		gradientValues[i*3  ]=GLubyte(127*cos(theta)*cos(phi)+127);
		gradientValues[i*3+1]=GLubyte(127*cos(theta)*sin(phi)+127);
		gradientValues[i*3+2]=GLubyte(127*sin(theta)+127);
	}

	glGenTextures(1, &gradientTexture);
	glBindTexture(GL_TEXTURE_3D, gradientTexture);
	glTexImage3D(	GL_TEXTURE_3D, 0, GL_RGBA8, 64, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE,
					gradientValues);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	//Done with the gradient values
	if(gradientValues)
		delete [] gradientValues;
	gradientValues=NULL;

	//Generate color lookup texture
	IMAGE colorImage;
	if(!colorImage.Load("colorlookup.tga"))
		return false;
	if(colorImage.paletted)
		colorImage.ExpandPalette();

	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_1D, colorTexture);
	glTexImage1D(	GL_TEXTURE_1D, 0, GL_RGBA8, colorImage.width, 0, colorImage.format,
					GL_UNSIGNED_BYTE, colorImage.data);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Generate weight factor lookup texture
	//(x, y, z) => (1-(3x^2 - 2x^3))*(1-(3y^2 - 2y^3))*(1-(3z^2 - 2z^3))

	//Create temporary space to hold weight values
	GLubyte * weightValues=new GLubyte[32*32*32];
	if(!weightValues)
	{
		LOG::Instance()->OutputError("Unable to create space for weight values");
		return false;
	}

	for(int i=0; i<32; ++i)
	{
		for(int j=0; j<32; ++j)
		{
			for(int k=0; k<32; ++k)
			{
				float x=float(i)/31;
				float y=float(j)/31;
				float z=float(k)/31;

				weightValues[(i*32+j)*32+k]=GLubyte(255.0f*	(1-(3*x*x - 2*x*x*x))*
															(1-(3*y*y - 2*y*y*y))*
															(1-(3*z*z - 2*z*z*z)));
			}
		}		
	}

	glGenTextures(1, &weightTexture);
	glBindTexture(GL_TEXTURE_3D, weightTexture);
	glTexImage3D(	GL_TEXTURE_3D, 0, GL_INTENSITY8, 32, 32, 32, 0, GL_LUMINANCE,
					GL_UNSIGNED_BYTE, weightValues);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	//Done with the weight values
	if(weightValues)
		delete [] weightValues;
	weightValues=NULL;

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

	//Handle pause/unpause
	if(WINDOW::Instance()->IsKeyPressed('P'))
	{
		timer.Pause();
		WINDOW::Instance()->SetKeyReleased('P');
	}

	if(WINDOW::Instance()->IsKeyPressed('U'))
	{
		timer.Unpause();
		WINDOW::Instance()->SetKeyReleased('U');
	}
	
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

	VECTOR3D cameraPosition(0.0f, 5.0f, 6.0f);

	gluLookAt(	cameraPosition.x, cameraPosition.y, cameraPosition.z,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	//Calculate the model matrix for the teapot
	MATRIX4X4 m1, m2, modelMatrix;
	m1.SetRotationY(timer.GetTime()/20);
	m2.SetRotationX(270.0);
	modelMatrix=m1*m2;
	

	//Bind and enable vertex/fragment programs
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, marbleVP);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, marbleFP);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);


	//Bind the gradient texture
	glBindTexture(GL_TEXTURE_3D, gradientTexture);

	//Bind the weight lookup texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, weightTexture);
	glActiveTexture(GL_TEXTURE0);

	//Bind the color lookup texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, colorTexture);
	glActiveTexture(GL_TEXTURE0);


	//Send the model matrix to vertex program local parameters 0-3
	glProgramLocalParameter4fvARB(	GL_VERTEX_PROGRAM_ARB, 0, modelMatrix.GetRow(0));
	glProgramLocalParameter4fvARB(	GL_VERTEX_PROGRAM_ARB, 1, modelMatrix.GetRow(1));
	glProgramLocalParameter4fvARB(	GL_VERTEX_PROGRAM_ARB, 2, modelMatrix.GetRow(2));
	glProgramLocalParameter4fvARB(	GL_VERTEX_PROGRAM_ARB, 3, modelMatrix.GetRow(3));

	//Send the camera position to vertex program local parameter 4
	glProgramLocalParameter4fvARB(	GL_VERTEX_PROGRAM_ARB, 4, VECTOR4D(cameraPosition));


	//Send the light direction to fragment program local parameter 0
	VECTOR3D lightDirection(1.0f, 1.0f, 1.0f);
	lightDirection.Normalize();
	glProgramLocalParameter4fvARB(	GL_FRAGMENT_PROGRAM_ARB, 0, VECTOR4D(lightDirection));


	//Draw the teapot
	glPushMatrix();
	glMultMatrixf(modelMatrix);
	
	Teapot(16, GL_FILL);

	glPopMatrix();


	glDisable(GL_VERTEX_PROGRAM_ARB);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	
	
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
