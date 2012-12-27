//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Quake 3 bsp
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
//#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
//#include <GL/wglext.h>
#include "LOG.h"
#include "WINDOW.h"
#include "extensions/ARB_multitexture_extension.h"
#include "extensions/EXT_draw_range_elements_extension.h"
#include "extensions/EXT_multi_draw_arrays_extension.h"
#include "extensions/EXT_texture_env_combine_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "main.h"
#include "BSP.h"
#include "WALKING_CAMERA.h"
#include "FRUSTUM.h"
#include "View.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

//errorLog MUST be kept - it is used by other files
LOG errorLog;
WINDOW window;
FPS_COUNTER fpsCounter;
TIMER timer;

COLOR backgroundColor(0.5f, 0.5f, 0.5f, 0.0f);

BSP bsp;

FRUSTUM frustum;

WALKING_CAMERA camera;

bool updatePVS=true;	//do we update the PVS every frame?

enum RENDER_METHOD	{SHOW_TEXTURES, SHOW_LIGHTMAPS, MODULATE_TEXTURES};
RENDER_METHOD renderMethod=MODULATE_TEXTURES;

//Set up variables
bool DemoInit()
{
	if(!window.Init((char*)"Quake 3 .bsp", 640, 480, 32, 24, 8, CHOOSE_SCREEN))
		return 0;											//quit if not created
	//hide cursor
//	ShowCursor(0);

//	SetUpARB_multitexture();
	SetUpEXT_texture_env_combine();
	SetUpEXT_draw_range_elements();
	SetUpEXT_multi_draw_arrays();

//	if(!ARB_multitexture_supported ||  !EXT_texture_env_combine_supported)
//		return false;

	//read in the map name etc from config.txt
	FILE * configFile=fopen("config.txt", "rt");
	if(!configFile)
	{
		errorLog.OutputError("Cannot open \"config.txt\"");
		return false;
	}

	char levelName[256];
	fscanf(configFile, "Map: %s\n", levelName);
	int curveTesselation;
	fscanf(configFile, "Curve Tesselation: %d\n", &curveTesselation);
	VECTOR3D cameraPosition;
	float angleYaw, anglePitch;
	fscanf(configFile, "Camera Position: (%f %f %f)\n",	&cameraPosition.x,
														&cameraPosition.y,
														&cameraPosition.z);
	fscanf(configFile, "Camera Orientation: %f %f\n", &angleYaw, &anglePitch);

	fclose(configFile);

	if(!bsp.Load(levelName, curveTesselation))
		return false;

	camera.Init(5.0f, cameraPosition, angleYaw, anglePitch);

	//reset timer for start
	timer.Reset();
	
	return true;
}

//Set up openGL
bool GLInit()
{
	//set viewport
	int height;
	if (window.height==0)
		height=1;
	else
		height=window.height;
	
	glViewport(0, 0, window.width, height);					//reset viewport

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);							//select projection matrix
	glLoadIdentity();										//reset
	gluPerspective(75.0f, (GLfloat)window.width/(GLfloat)height, 0.1f, 100.0f);
	
	//load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//other states
	//shading
	glShadeModel(GL_SMOOTH);
	glClearColor(	backgroundColor.r,
					backgroundColor.g,
					backgroundColor.b,
					backgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_CULL_FACE);

	return true;
}

//Perform per frame updates
void UpdateFrame()
{
	window.Update();
	camera.Update(timer.GetTime());

	//Pause/Unpause PVS updates
	if(window.isKeyPressed('P'))
		updatePVS=false;

	if(window.isKeyPressed('U'))
		updatePVS=true;

	//Toggle render method
	if(window.isKeyPressed('T'))
		renderMethod=SHOW_TEXTURES;

	if(window.isKeyPressed('L'))
		renderMethod=SHOW_LIGHTMAPS;

	if(window.isKeyPressed('M'))
		renderMethod=MODULATE_TEXTURES;
}

//draw a frame
void RenderFrame()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix

	glRotatef(camera.anglePitch, 1.0f, 0.0f, 0.0f);
	glRotatef(camera.angleYaw, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera.position.x, -camera.position.y, -camera.position.z);

	frustum.Update();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	//Set states for drawing map
	//Set up texture units

	//Unit 0 - replace with decal textures
	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	//Unit 1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);

	if(renderMethod==MODULATE_TEXTURES)	//Then modulate by lightmap, then double
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2.0f);
	}

	if(renderMethod==SHOW_TEXTURES)	//Then replace with previous
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	}

	if(renderMethod==SHOW_LIGHTMAPS)//Then replace with lightmaps
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	}
	
	glActiveTextureARB(GL_TEXTURE0_ARB);

	if(updatePVS)
		bsp.CalculateVisibleFaces(camera.position, frustum);
	
	bsp.Draw();

	glPopAttrib();


	fpsCounter.Update();											//update frames per second counter
	window.StartTextMode();
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	if(renderMethod==SHOW_TEXTURES)
		window.Print(0, 48, "Showing Textures");
	if(renderMethod==SHOW_LIGHTMAPS)
		window.Print(0, 48, "Showing Lightmaps");
	if(renderMethod==MODULATE_TEXTURES)
		window.Print(0, 48, "Showing Lit Textures");
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	if(!updatePVS)
		window.Print(0, 68, "PVS/Frustum Cull paused");
	window.EndTextMode();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

//	if(window.isKeyPressed(VK_F1))
//	{
//		window.SaveScreenshot();
//		window.SetKeyReleased(VK_F1);
//	}

	window.SwapBuffers();									//swap buffers

	//check for any opengl errors
	window.CheckGLError();

	//quit if necessary
//	if(window.isKeyPressed(VK_ESCAPE))
//		PostQuitMessage(0);
}

void DemoShutdown()
{
	window.Shutdown();										//Shutdown window
//	ShowCursor(1);
}

int main(int argc, char **argv) {
	View * v = new View(&argc, argv);
	v->createWindow("q3bsp", 640, 480);
	//Initiation
	errorLog.Init("Error Log.txt");

	//init variables etc, then GL
	if(!DemoInit())
	{
		errorLog.OutputError("Demo Initiation failed");
		return 0;
	}
	else
		errorLog.OutputSuccess("Demo Initiation Successful");

	if(!GLInit())
	{
		errorLog.OutputError("OpenGL Initiation failed");
		return 0;
	}
	else
		errorLog.OutputSuccess("OpenGL Initiation Successful");

//	//Main Loop
//	for(;;)
//	{
//		if(!(window.HandleMessages())) break;//handle windows messages, quit if returns false
//		UpdateFrame();
//		RenderFrame();
//	}

	DemoShutdown();
	
	errorLog.OutputSuccess("Exiting...");
	return 0;								//Exit The Program
}
