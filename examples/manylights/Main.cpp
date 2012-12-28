//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Many Lights
//	Downloaded from: www.paulsprojects.net
//	Created:	15th January 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>	//for rand()
#include <time.h>	//for time in srand
#include "GL files/glee.h"	//header for opengl versions >1.1 and extensions
#include <GL/glu.h>
#include "Maths/Maths.h"
#include "Log/LOG.h"
#include "Timer/TIMER.h"
#include "Fps Counter/FPS_COUNTER.h"
#include "Window/WINDOW.h"
#include "Bitmap Font/BITMAP_FONT.h"
#include "Image/IMAGE.h"
#include "GL files/ARB_program.h"
#include "Main.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

TIMER timer;
FPS_COUNTER fpsCounter;
BITMAP_FONT font;

COLOR backgroundColor(0.4f, 0.0f, 0.5f, 0.0f);

//Grid
int gridDensity=64;
int numVertices;
SIMPLE_VERTEX * vertices=NULL;
int numIndices;
GLuint * indices=NULL;

GLuint floorTexture;

//Lights
const int numLights=60;
POINT_LIGHT lights[numLights];

//Vertex programs
bool useVP1=false;
GLuint vp1;	//NV_vertex_program

bool useVP2=false;
GLuint vp2;	//NV_vertex_program2

//Use the fixed function pipeline?
bool useFixedFunction=true;

//Set up GL
bool GLInit()
{
	//Init window
	if(!WINDOW::Instance()->Init("Many Lights", 640, 480, 8, 8, 8, 8, 24, 8, false, false))
		return false;
	
	//Check for NV_vertex_program
	if(GLEE_NV_vertex_program)
	{
		//default to vp1
		useVP1=true;
		useVP2=false;
		useFixedFunction=false;
	}

	//Check for NV_vertex_program2
	if(GLEE_NV_vertex_program2)
	{
		//default to vp2
		useVP1=false;
		useVP2=true;
		useFixedFunction=false;
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

	glEnable(GL_CULL_FACE);

	return true;
}

//Set up variables
bool DemoInit()
{
	//Seed random number generator	
	srand( (unsigned)time( NULL ) );

	//Initialise the array of vertices
	numVertices=gridDensity*gridDensity;
	vertices=new SIMPLE_VERTEX[numVertices];
	if(!vertices)
	{
		LOG::Instance()->OutputError("Unable to allocate space for %d vertices", numVertices);
		return false;
	}

	for(int i=0; i<gridDensity; ++i)
	{
		for(int j=0; j<gridDensity; ++j)
		{
			vertices[i*gridDensity+j].position.Set(	(float(i)/(gridDensity-1))*2-1,
													0.0f,
													(float(j)/(gridDensity-1))*2-1);
			
			vertices[i*gridDensity+j].normal.Set(	0.0f, 1.0f, 0.0f);
			
			vertices[i*gridDensity+j].texCoords.Set( (float(i)/(gridDensity-1)),
													-(float(j)/(gridDensity-1)));
		}
	}

	//Initialise the indices
	numIndices=2*(gridDensity)*(gridDensity-1);
	indices=new GLuint[numIndices];
	if(!indices)
	{
		LOG::Instance()->OutputError("Unable to allocate space for %d indices", numIndices);
		return false;
	}

	for(int i=0; i<gridDensity-1; ++i)
	{
		for(int j=0; j<gridDensity; ++j)
		{
			indices[(i*gridDensity+j)*2  ]=(i+1)*gridDensity+j;
			indices[(i*gridDensity+j)*2+1]=i*gridDensity+j;
		}
	}

	//Load texture
	IMAGE floorImage;
	floorImage.Load("OpenGL.tga");
	if(floorImage.paletted)
		floorImage.ExpandPalette();
	
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(	GL_TEXTURE_2D, GL_RGBA8, floorImage.width, floorImage.height,
						floorImage.format, GL_UNSIGNED_BYTE, floorImage.data);

	//Initialise the lights
	for(int i=0; i<numLights; ++i)
	{
		lights[i].position.Set(	(float(rand())/RAND_MAX)*2-1,
								0.02f,
								(float(rand())/RAND_MAX)*2-1);
		lights[i].velocity.Set(	(float(rand())/RAND_MAX)*2-1,
								0.0f,
								(float(rand())/RAND_MAX)*2-1);
		lights[i].color.Set(	(float(rand())/RAND_MAX)*0.75f,
								(float(rand())/RAND_MAX)*0.75f,
								(float(rand())/RAND_MAX)*0.75f,
								1.0f);
	}

	//Load vertex programs
	if(GLEE_NV_vertex_program)
	{
		glGenProgramsNV(1, &vp1);
		glBindProgramNV(GL_VERTEX_PROGRAM_NV, vp1);
		LoadARB_program(GL_VERTEX_PROGRAM_NV, "vp1.txt");
	}

	if(GLEE_NV_vertex_program2)
	{
		glGenProgramsNV(1, &vp2);
		glBindProgramNV(GL_VERTEX_PROGRAM_NV, vp2);
		LoadARB_program(GL_VERTEX_PROGRAM_NV, "vp2.txt");
	}

	if(GLEE_NV_vertex_program || GLEE_NV_vertex_program2)
	{
		//Track modelview-projection matrix
		glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
	}

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

	//Update the lights
	for(int i=0; i<numLights; ++i)
	{
		lights[i].position+=lights[i].velocity*float(timePassed)/1000;

		//bounce off the borders
		if(lights[i].position.x>1.0f && lights[i].velocity.x>0.0f)
			lights[i].velocity.x=-lights[i].velocity.x;
		if(lights[i].position.x<-1.0f && lights[i].velocity.x<0.0f)
			lights[i].velocity.x=-lights[i].velocity.x;

		if(lights[i].position.z>1.0f && lights[i].velocity.z>0.0f)
			lights[i].velocity.z=-lights[i].velocity.z;
		if(lights[i].position.z<-1.0f && lights[i].velocity.z<0.0f)
			lights[i].velocity.z=-lights[i].velocity.z;
	}
	
	//Toggle vertex program/fixed function
	if(WINDOW::Instance()->IsKeyPressed('1') && GLEE_NV_vertex_program)
	{
		useVP1=true;
		useVP2=false;
		useFixedFunction=false;
	}
	
	if(WINDOW::Instance()->IsKeyPressed('2') && GLEE_NV_vertex_program2)
	{
		useVP1=false;
		useVP2=true;
		useFixedFunction=false;
	}

	if(WINDOW::Instance()->IsKeyPressed('3'))
	{
		useVP1=false;
		useVP2=false;
		useFixedFunction=true;
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

	gluLookAt(	0.0f, 1.8f, 1.8f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0);

	//Enable texture
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glEnable(GL_TEXTURE_2D);

	//Set up vertex arrays
	glVertexPointer(3, GL_FLOAT, sizeof(SIMPLE_VERTEX), &vertices[0].position);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glNormalPointer(GL_FLOAT, sizeof(SIMPLE_VERTEX), &vertices[0].normal);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glTexCoordPointer(2, GL_FLOAT, sizeof(SIMPLE_VERTEX), &vertices[0].texCoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	

	
	//Use Fixed Function Pipeline
	if(useFixedFunction)
	{
		//Set parameters for GL lighting
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white*0.1f);	//global ambient
		glMaterialfv(GL_FRONT, GL_AMBIENT, white);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white);

		glEnable(GL_LIGHTING);

		//8 passes, dealing with 8 lights at a time, except final pass where we do 4
		for(int i=0; i<8; ++i)
		{
			int numLightsThisPass=8;
			if(i==7)
				numLightsThisPass=4;

			//Set up lights
			for(int j=0; j<numLightsThisPass; ++j)
			{
				glLightfv(GL_LIGHT0+j, GL_POSITION, VECTOR4D(lights[i*8+j].position));
				glLightfv(GL_LIGHT0+j, GL_DIFFUSE, lights[i*8+j].color);
				glEnable(GL_LIGHT0+j);
			}

			//If this is the final pass, disable lights 4-7
			if(i==7)
				for(int j=4; j<7; ++j)
					glDisable(GL_LIGHT0+j);
			
			//Enable additive blend for all passes except the first
			if(i!=0)
			{
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);

				//if this is not the first pass, no ambient
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);	
			}

			//Draw grid as triangle strips
			for(int j=0; j<gridDensity-1; ++j)
				glDrawElements(	GL_TRIANGLE_STRIP, gridDensity*2,
								GL_UNSIGNED_INT, &indices[j*gridDensity*2]);
		}

		//Disable lights
		for(int j=0; j<7; ++j)
			glDisable(GL_LIGHT0+j);

		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
	}




	//Use vp1
	if(useVP1)
	{
		//Bind program
		glBindProgramNV(GL_VERTEX_PROGRAM_NV, vp1);
		glEnable(GL_VERTEX_PROGRAM_NV);

		//3 passes, dealing with 20 lights at a time 
		for(int i=0; i<3; ++i)
		{
			//Send ambient brightness as c[4]
			//Only send this for the first pass
			if(i==0)
				glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 4, white*0.1f);
			else
				glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 4, black);

			//Update program parameters 5&6 for light0, 7&8 for light1, etc
			for(int j=0; j<20; ++j)
			{
				glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 2*j+5,
										VECTOR4D(lights[i*20+j].position));
				glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 2*j+6,
										lights[i*20+j].color);
			}

			//Enable additive blend for passes except the first
			if(i>0)
			{
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
			}

			//Draw grid as triangle strips
			for(int j=0; j<gridDensity-1; ++j)
				glDrawElements(	GL_TRIANGLE_STRIP, gridDensity*2,
								GL_UNSIGNED_INT, &indices[j*gridDensity*2]);
		}

		glDisable(GL_VERTEX_PROGRAM_NV);
		glDisable(GL_BLEND);
	}



	//Use vp2
	if(useVP2)
	{
		//Bind program
		glBindProgramNV(GL_VERTEX_PROGRAM_NV, vp2);
		glEnable(GL_VERTEX_PROGRAM_NV);

		//Send ambient brightness as c[4]
		glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 4, white*0.1f);
		
		//Update program parameters 5&6 for light0, 7&8 for light1, etc
		for(int j=0; j<60; ++j)
		{
			glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 2*j+5,
									VECTOR4D(lights[j].position));
			glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 2*j+6,
									lights[j].color);
		}

		//Send loop parameters to c[125]
		glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 125,	60.0,	//number of lights
															 5.0,	//base offset of light data
															-1.0,
															 2.0);//num parameters between lights

		//Draw grid as triangle strips
		for(int j=0; j<gridDensity-1; ++j)
			glDrawElements(	GL_TRIANGLE_STRIP, gridDensity*2,
							GL_UNSIGNED_INT, &indices[j*gridDensity*2]);

		glDisable(GL_VERTEX_PROGRAM_NV);
	}





	//reset states
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	

	fpsCounter.Update();

	//Print text
	font.StartTextMode();
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	font.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
	if(useVP1)
		font.Print(0, 48, "Using NV_vertex_program");
	if(useVP2)
		font.Print(0, 48, "Using NV_vertex_program2");
	if(useFixedFunction)
		font.Print(0, 48, "Using Fixed Function Pipeline");
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

	if(vertices)
		delete [] vertices;
	vertices=NULL;

	if(indices)
		delete [] indices;
	indices=NULL;
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
