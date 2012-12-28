//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Deferred Shading
//	Downloaded from: www.paulsprojects.net
//	Created:	14th August 2003
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
#include "GL Files/LoadProgram.h"
#include "PBUFFER.h"
#include "TORUS_KNOT.h"
#include "CIRCULAR_PATH.h"
#include "POINT_LIGHT.h"
#include "Main.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

TIMER timer;
FPS_COUNTER fpsCounter;
BITMAP_FONT font;

COLOR backgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

//Torus knot and associated vertex/index buffer objects
TORUS_KNOT torusKnot;
GLuint torusKnotVBO;
GLuint torusKnotIBO;

//Lights
const int numLights=8;
POINT_LIGHT lights[numLights];

//Light texture
IMAGE lightImage;
GLuint lightTexture;

//Shading mode
enum SHADING_MODE {DEFERRED_SHADING, STANDARD_SHADING};
SHADING_MODE shadingMode=DEFERRED_SHADING;

//Vertex/fragment programs for standard shading
GLuint standardShadingVP;
GLuint standardShadingFP;

//Vertex/fragment programs for deferred shading
GLuint deferredShadingPass1VP;
GLuint deferredShadingPass1FP;
GLuint deferredShadingPass2FP;

//Floating point pBuffer and associated texture
PBUFFER pbuffer;
GLuint pbufferTexture;

//Camera details - needed for scissor rectangle etc calculation
const float cameraNearDistance=1.0f;
const float cameraFovy=45.0f;
float cameraAspectRatio;	//set once window size is known

//Set up GL
bool GLInit()
{
	//Init window
	if(!WINDOW::Instance()->Init("Deferred Shading", 640, 480, 8, 8, 8, 8, 24, 8, false, false))
		return false;
	
	//Check for OpenGL version/extensions (pbuffer setup checks for WGL_ARB_pbuffer etc)
	if(	!GLEE_ARB_fragment_program		||
		!GLEE_ARB_vertex_buffer_object	|| 
		!GLEE_ARB_vertex_program		||
		!GLEE_NV_float_buffer			||
		!GLEE_NV_fragment_program		||
		!GLEE_NV_texture_rectangle)
	{
		LOG::Instance()->OutputError("I require ARB_fragment_program, ARB_vertex_buffer_object,");
		LOG::Instance()->OutputError("ARB_vertex_program, NV_float_buffer,");
		LOG::Instance()->OutputError("NV_fragment_program and NV_texture_rectangle extensions");
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

	cameraAspectRatio=(float)WINDOW::Instance()->width/WINDOW::Instance()->height;

	gluPerspective(cameraFovy, cameraAspectRatio, cameraNearDistance, 100.0f);

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
	//Initialise torus knot
	torusKnot.Init(7, 64, 512);

	//Generate and fill the VBO for the torus knot
	glGenBuffersARB(1, &torusKnotVBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, torusKnotVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, torusKnot.numVertices*sizeof(TORUS_KNOT_VERTEX),
					torusKnot.vertices, GL_STATIC_DRAW_ARB);

	//Generate and fill the IBO for the torus knot indices
	glGenBuffersARB(1, &torusKnotIBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, torusKnotIBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, torusKnot.numIndices*sizeof(unsigned int),
					torusKnot.indices, GL_STATIC_DRAW_ARB);

	//Set up vertex arrays for torus knot
	glVertexPointer(3, GL_FLOAT, sizeof(TORUS_KNOT_VERTEX), (char *)NULL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(TORUS_KNOT_VERTEX), (char *)NULL + 12);
	glEnableClientState(GL_NORMAL_ARRAY);

	//Initialise the lights
	if(	!lights[0].Init(VECTOR3D(0.0f, 0.0f, 0.0f), VECTOR3D(2.0f, 0.0f, 0.0f),
						VECTOR3D(0.0f, 0.0f,-2.0f), 0.05f, COLOR(0.5f, 0.2f, 0.4f), 2.0f)	||

		!lights[1].Init(VECTOR3D(0.0f, 0.0f, 0.0f), VECTOR3D(0.0f, 0.0f, 2.0f),
						VECTOR3D(2.0f, 0.0f, 0.0f), 0.05f, COLOR(0.5f, 0.2f, 0.4f), 2.0f)	||
		
		!lights[2].Init(VECTOR3D(0.0f, 0.0f, 0.0f), VECTOR3D(-2.0f, 0.0f, 0.0f),
						VECTOR3D(0.0f, 0.0f, 2.0f), 0.05f, COLOR(0.5f, 0.2f, 0.4f), 2.0f)	||

		!lights[3].Init(VECTOR3D(0.0f, 0.0f, 0.0f), VECTOR3D(0.0f, 0.0f,-2.0f),
						VECTOR3D(-2.0f, 0.0f, 0.0f), 0.05f, COLOR(0.5f, 0.2f, 0.4f), 2.0f)	||
		
		!lights[4].Init(VECTOR3D(0.0f, 1.0f, 0.0f), VECTOR3D(1.0f, 1.0f, 0.0f),
						VECTOR3D(0.0f, 1.0f, 1.0f), 0.05f, COLOR(0.0f, 1.0f, 0.0f), 1.5f)	||
		
		!lights[5].Init(VECTOR3D(0.0f, 1.0f, 0.0f), VECTOR3D(0.0f, 1.0f, 1.0f),
						VECTOR3D(-1.0f, 1.0f, 0.0f), 0.05f, COLOR(1.0f, 0.0f, 0.0f), 1.5f)	||
				
		!lights[6].Init(VECTOR3D(0.0f, 1.0f, 0.0f), VECTOR3D(-1.0f, 1.0f, 0.0f),
						VECTOR3D(0.0f, 1.0f,-1.0f), 0.05f, COLOR(0.3f, 0.3f, 1.0f), 1.5f)	||
		
		!lights[7].Init(VECTOR3D(0.0f, 1.0f, 0.0f), VECTOR3D(0.0f, 1.0f,-1.0f),
						VECTOR3D(1.0f, 1.0f, 0.0f), 0.05f, COLOR(0.7f, 0.7f, 0.0f), 1.5f))
	{
		return false;
	}

	//Load the light texture
	if(!lightImage.Load("light.tga"))
		return false;
		
	glGenTextures(1, &lightTexture);
	glBindTexture(GL_TEXTURE_2D, lightTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA8, lightImage.width, lightImage.height, 0,
					lightImage.format, GL_UNSIGNED_BYTE, lightImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Load the vertex and fragment programs
	glGenProgramsARB(1, &standardShadingVP);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, standardShadingVP);
	if(!LoadProgram(GL_VERTEX_PROGRAM_ARB, "StandardShadingVP.txt"))
		return false;

	glGenProgramsARB(1, &standardShadingFP);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, standardShadingFP);
	if(!LoadProgram(GL_FRAGMENT_PROGRAM_ARB, "StandardShadingFP.txt"))
		return false;

	glGenProgramsARB(1, &deferredShadingPass1VP);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, deferredShadingPass1VP);
	if(!LoadProgram(GL_VERTEX_PROGRAM_ARB, "DeferredShadingPass1VP.txt"))
		return false;

	glGenProgramsNV(1, &deferredShadingPass1FP);
	glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass1FP);
	if(!LoadProgram(GL_FRAGMENT_PROGRAM_NV, "DeferredShadingPass1FP.txt"))
		return false;

	glGenProgramsNV(1, &deferredShadingPass2FP);
	glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass2FP);
	if(!LoadProgram(GL_FRAGMENT_PROGRAM_NV, "DeferredShadingPass2FP.txt"))
		return false;
	
	//Create the pBuffer
	const int pbufferAttribIList[]={	WGL_DRAW_TO_PBUFFER_ARB,	true,
										WGL_RED_BITS_ARB,			32,
										WGL_GREEN_BITS_ARB,			32,
										WGL_BLUE_BITS_ARB,			32,
										WGL_ALPHA_BITS_ARB,			32,
										WGL_FLOAT_COMPONENTS_NV,	true,
										WGL_DEPTH_BITS_ARB,			24,
										WGL_STENCIL_BITS_ARB,		8,
										0};

	const float pbufferAttribFList[]={0};
	const int pbufferFlags[]={0};
	
	if(!pbuffer.Init(	WINDOW::Instance()->width, WINDOW::Instance()->height,
						pbufferAttribIList, pbufferAttribFList, pbufferFlags))
	{
		LOG::Instance()->OutputError("Unable to create pbuffer");
		return false;
	}

	//Share textures & programs between window and pbuffer contexts
	if(!wglShareLists(WINDOW::Instance()->hRC, pbuffer.hRC))
	{
		LOG::Instance()->OutputError("Unable to share lists between window and pbuffer contexts");
		return false;
	}

	//Initialise the pbuffer context
	pbuffer.MakeCurrent();
	{
		glViewport(0, 0, pbuffer.width, pbuffer.height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(cameraFovy, cameraAspectRatio, cameraNearDistance, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glShadeModel(GL_SMOOTH);
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glClearDepth(1.0f);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);

		//Pbuffer context also needs vertex arrays for torus knot
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, torusKnotVBO);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, torusKnotIBO);
	
		glVertexPointer(3, GL_FLOAT, sizeof(TORUS_KNOT_VERTEX), (char *)NULL);
		glEnableClientState(GL_VERTEX_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(TORUS_KNOT_VERTEX), (char *)NULL + 12);
		glEnableClientState(GL_NORMAL_ARRAY);
	}
	WINDOW::Instance()->MakeCurrent();

	//Initialise the pbuffer texture
	glGenTextures(1, &pbufferTexture);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, pbufferTexture);
	glTexImage2D(	GL_TEXTURE_RECTANGLE_NV, 0, GL_FLOAT_RGBA32_NV, WINDOW::Instance()->width,
					WINDOW::Instance()->height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

	//Toggle between deferred shading and standard
	if(WINDOW::Instance()->IsKeyPressed('1'))
	{
		shadingMode=DEFERRED_SHADING;
		WINDOW::Instance()->SetKeyReleased('1');
	}

	if(WINDOW::Instance()->IsKeyPressed('2'))
	{
		shadingMode=STANDARD_SHADING;
		WINDOW::Instance()->SetKeyReleased('2');
	}

	//Toggle Pause
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

	gluLookAt(	0.0f, 4.0f, 3.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	//Save the view matrix
	MATRIX4X4 viewMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);	

	if(shadingMode==DEFERRED_SHADING)
	{
		//Pass 1
		//Draw the geometry, saving parameters into the buffer

		//Make the pbuffer the current context
		pbuffer.MakeCurrent();

		//Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glLoadIdentity();
		gluLookAt(	0.0f, 4.0f, 3.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f);

		//Bind and enable vertex & fragment programs
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, deferredShadingPass1VP);
		glEnable(GL_VERTEX_PROGRAM_ARB);
	
		glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass1FP);
		glEnable(GL_FRAGMENT_PROGRAM_NV);

		//Draw the torus knot
		glDrawElements(GL_TRIANGLES, torusKnot.numIndices, GL_UNSIGNED_INT, (char *)NULL);

		//Draw the "floor"
		glNormal3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		{
			glVertex3f( 5.0f,-0.5f, 5.0f);
			glVertex3f( 5.0f,-0.5f,-5.0f);
			glVertex3f(-5.0f,-0.5f, 5.0f);
			glVertex3f(-5.0f,-0.5f,-5.0f);
		}
		glEnd();
		
		glDisable(GL_VERTEX_PROGRAM_ARB);
		glDisable(GL_FRAGMENT_PROGRAM_NV);

		//Copy the pbuffer contents into the pbuffer texture
		glBindTexture(GL_TEXTURE_RECTANGLE_NV, pbufferTexture);
		glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0,
							pbuffer.width, pbuffer.height);

		//Make the window the current context
		WINDOW::Instance()->MakeCurrent();

		//Pass 2
		//Draw a quad covering the region of influence of each light
		//Unpack the data from the buffer, perform the lighting equation and update
		//the framebuffer
		
		//Set orthographic projection, 1 unit=1 pixel
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, WINDOW::Instance()->width, 0, WINDOW::Instance()->height);
		
		//Set identity modelview
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//Disable depth test
		glDisable(GL_DEPTH_TEST);

		//Bind the pbuffer texture
		glBindTexture(GL_TEXTURE_RECTANGLE_NV, pbufferTexture);

		//Bind and enable fragment program
		glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, deferredShadingPass2FP);
		glEnable(GL_FRAGMENT_PROGRAM_NV);

		//Loop through the lights
		for(int i=0; i<numLights; ++i)
		{
			//Calculate the rectangle to draw for this light
			int rectX, rectY, rectWidth, rectHeight;

			lights[i].GetWindowRect(WINDOW::Instance()->width, WINDOW::Instance()->height,
									viewMatrix, currentTime, cameraNearDistance,
									cameraFovy, cameraAspectRatio,
									rectX, rectY, rectWidth, rectHeight);

			//Enable additive blend if i>0
			if(i>0)
			{
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
			}

			//Send the light's color to fragment program local parameter 0
			glProgramLocalParameter4fvARB(	GL_FRAGMENT_PROGRAM_NV, 0, lights[i].color);

			//Send 1/(light radius)^2 to fragment program local parameter 1
			float inverseSquareLightRadius=1.0f/(lights[i].radius*lights[i].radius);
			glProgramLocalParameter4fARB(	GL_FRAGMENT_PROGRAM_NV, 1,
											inverseSquareLightRadius, inverseSquareLightRadius,
											inverseSquareLightRadius, inverseSquareLightRadius);

			//Send the light's position to fragment program local parameter 2
			glProgramLocalParameter4fvARB(	GL_FRAGMENT_PROGRAM_NV, 2,
											VECTOR4D(lights[i].GetPosition(currentTime)));
			
			//Draw the rectangle
			glBegin(GL_TRIANGLE_STRIP);
			{
				glVertex2i(rectX,			rectY);
				glVertex2i(rectX+rectWidth,	rectY);
				glVertex2i(rectX,			rectY+rectHeight);
				glVertex2i(rectX+rectWidth,	rectY+rectHeight);
			}
			glEnd();
		}

		//Restore matrices
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_FRAGMENT_PROGRAM_NV);
		glDisable(GL_BLEND);
	}
	else	//STANDARD_SHADING
	{
		//Make an initial pass to lay down Z
		glColorMask(0, 0, 0, 0);

		//Draw the torus knot
		glDrawElements(GL_TRIANGLES, torusKnot.numIndices, GL_UNSIGNED_INT, (char *)NULL);

		//Draw the "floor"
		glNormal3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP);
		{
			glVertex3f( 5.0f,-0.5f, 5.0f);
			glVertex3f( 5.0f,-0.5f,-5.0f);
			glVertex3f(-5.0f,-0.5f, 5.0f);
			glVertex3f(-5.0f,-0.5f,-5.0f);
		}
		glEnd();

		glColorMask(1, 1, 1, 1);

		//Bind and enable vertex & fragment programs
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, standardShadingVP);
		glEnable(GL_VERTEX_PROGRAM_ARB);
	
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, standardShadingFP);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		//Loop through the lights
		for(int i=0; i<numLights; ++i)
		{
			//Calculate and set the scissor rectangle for this light
			int scissorX, scissorY, scissorWidth, scissorHeight;

			lights[i].GetWindowRect(WINDOW::Instance()->width, WINDOW::Instance()->height,
									viewMatrix, currentTime, cameraNearDistance,
									cameraFovy, cameraAspectRatio,
									scissorX, scissorY, scissorWidth, scissorHeight);

			glScissor(scissorX, scissorY, scissorWidth, scissorHeight);
			glEnable(GL_SCISSOR_TEST);

			//Enable additive blend if i>0
			if(i>0)
			{
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
			}

			//Calculate the object space light position and send to
			//vertex program local parameter 0
			//Object space and world space are the same
			glProgramLocalParameter4fvARB(	GL_VERTEX_PROGRAM_ARB, 0,
											VECTOR4D(lights[i].GetPosition(currentTime)));

			//Send the light's color to fragment program local parameter 0
			glProgramLocalParameter4fvARB(	GL_FRAGMENT_PROGRAM_ARB, 0, lights[i].color);

			//Send 1/(light radius)^2 to fragment program local parameter 1
			float inverseSquareLightRadius=1.0f/(lights[i].radius*lights[i].radius);
			glProgramLocalParameter4fARB(	GL_FRAGMENT_PROGRAM_ARB, 1,
											inverseSquareLightRadius, inverseSquareLightRadius,
											inverseSquareLightRadius, inverseSquareLightRadius);

			//Draw the torus knot
			glDrawElements(GL_TRIANGLES, torusKnot.numIndices, GL_UNSIGNED_INT, (char *)NULL);

			//Draw the "floor"
			glNormal3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			{
				glVertex3f( 5.0f,-0.5f, 5.0f);
				glVertex3f( 5.0f,-0.5f,-5.0f);
				glVertex3f(-5.0f,-0.5f, 5.0f);
				glVertex3f(-5.0f,-0.5f,-5.0f);
			}
			glEnd();
		}

		glDisable(GL_VERTEX_PROGRAM_ARB);
		glDisable(GL_FRAGMENT_PROGRAM_ARB);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_BLEND);
	}

	
	//Extract the right and up vectors from the view matrix
	VECTOR3D right(	viewMatrix.entries[0],
					viewMatrix.entries[4], 
					viewMatrix.entries[8]);
	VECTOR3D up(	viewMatrix.entries[1],
					viewMatrix.entries[5], 
					viewMatrix.entries[9]);

	//Draw the lights as billboarded quads
	glBindTexture(GL_TEXTURE_2D, lightTexture);
	glEnable(GL_TEXTURE_2D);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);

	for(int i=0; i<numLights; ++i)
	{
		float lightSize=0.2f;

		glColor4fv(lights[i].color);
		VECTOR3D lightPosition=lights[i].GetPosition(currentTime);

		glBegin(GL_TRIANGLE_STRIP);
		{
			glTexCoord2f(0.0f, 1.0f);
			glVertex3fv(lightPosition+(-right+up)*lightSize);
			
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(lightPosition+(-right-up)*lightSize);
			
			glTexCoord2f(1.0f, 1.0f);
			glVertex3fv(lightPosition+(right+up)*lightSize);
			
			glTexCoord2f(1.0f, 0.0f);
			glVertex3fv(lightPosition+(right-up)*lightSize);
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glColor4fv(white);

	
	fpsCounter.Update();

	//Print text
	font.StartTextMode();
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	font.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	if(shadingMode==DEFERRED_SHADING)
		font.Print(0, 48, "Deferred Shading");
	else
		font.Print(0, 48, "Standard Shading");
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
	//Delete the torus VBO & IBO
	glDeleteBuffersARB(1, &torusKnotVBO);
	glDeleteBuffersARB(1, &torusKnotIBO);

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
