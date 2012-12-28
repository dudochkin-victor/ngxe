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
#include <string.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
//#include <GL/wglext.h>
#include "LOG.h"
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
FPS_COUNTER fpsCounter;
TIMER timer;

COLOR backgroundColor(0.5f, 0.5f, 0.5f, 0.0f);

BSP bsp;

FRUSTUM frustum;

WALKING_CAMERA camera;
GLuint base;					//display list base for font
GLuint startTextModeList;

bool updatePVS = true;	//do we update the PVS every frame?

enum RENDER_METHOD {
	SHOW_TEXTURES, SHOW_LIGHTMAPS, MODULATE_TEXTURES
};
RENDER_METHOD renderMethod = MODULATE_TEXTURES;

//Text writing functions
void StartTextMode(void)
{
	int width = 640;
	int height = 480;
	//If not yet created, make display list
	if(!startTextModeList)
	{
		startTextModeList=glGenLists(1);
		glNewList(startTextModeList, GL_COMPILE);
		{
			//save states
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glListBase(base-32);							//set the list base

			//set modelview matrix
			glPushMatrix();
			glLoadIdentity();

			//set projection matrix
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

			//set states
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glBlendFunc(GL_ONE, GL_ONE);	//if blending, use additive
		}
		glEndList();
	}

	glCallList(startTextModeList);
}

void Print(int x, int y, const char * string, ...)
{
	char text[256];									//Holds our string
	va_list va;										//pointer to list of arguments

	if(string==NULL)								//If there's no text
		return;										//Do nothing

	va_start(va, string);							//parse string for variables
		vsprintf(text, string, va);					//convert to actual numbers
	va_end(va);										//results stored in text

	glRasterPos2i(x, y);								//go to correct raster position

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	//call display lists
}

void EndTextMode(void)
{
	//restore states
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
}

void CheckGLError(void)
{
	GLenum error;
	error=glGetError();
	if(!(error==GL_NO_ERROR))
	{
		errorLog.OutputError("OpenGL Error:");
		if(error==GL_INVALID_ENUM)
		{
			errorLog.OutputError("	GL_INVALID_ENUM");
			errorLog.OutputError("	GLenum Argument out of range.");
		}
		if(error==GL_INVALID_VALUE)
		{
			errorLog.OutputError("	GL_INVALID_VALUE");
			errorLog.OutputError("	Numeric Argument out of range.");
		}
		if(error==GL_INVALID_OPERATION)
		{
			errorLog.OutputError("	GL_INVALID_OPERATION");
			errorLog.OutputError("	Invalid Operation in current state.");
		}
		if(error==GL_STACK_UNDERFLOW)
		{
			errorLog.OutputError("	GL_STACK_UNDERFLOW");
			errorLog.OutputError("	Stack Underflow.");
		}
		if(error==GL_STACK_OVERFLOW)
		{
			errorLog.OutputError("	GL_STACK_OVERFLOW");
			errorLog.OutputError("	Stack Overflow.");
		}
		if(error==GL_OUT_OF_MEMORY)
		{
			errorLog.OutputError("	GL_OUT_OF_MEMORY");
			errorLog.OutputError("	Out of memory.");
		}
	}
}
void SaveScreenshot(void)
{
	FILE * file;

	//first calculate the filename to save to
	char filename[32];

	for(int i=0; i<1000; i++)
	{
		sprintf(filename, "screen%03d.tga", i);

		//try opening this file - if not possible, use this filename
		file=fopen(filename, "rb");

		if(!file)
		{
			break;
		}

		//otherwise, the file exists, try next, except if this is the last one
		fclose(file);

		if(i==999)
		{
			errorLog.OutputError("No space to save screenshot - 0-999 exist");
			return;
		}
	}

	errorLog.OutputSuccess("Saving %s", filename);

	GLubyte		TGAheader[12]={0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};	//Uncompressed TGA header
	GLubyte		infoHeader[6];

	int width = 640;
	int height = 480;
	unsigned char * data=new unsigned char[4*width*height];
	if(!data)
	{
		errorLog.OutputError("Unable to allocate memory for screen data");
		return;
	}

	//read in the screen data
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//data needs to be in BGR format
	//swap b and r
	for(int i=0; i<(int)width*height*4; i+=4)
	{
		//repeated XOR to swap bytes 0 and 2
		data[i] ^= data[i+2] ^= data[i] ^= data[i+2];
	}

	//open the file
	file = fopen(filename, "wb");

	//save header
	fwrite(TGAheader, 1, sizeof(TGAheader), file);

	//save info header
	infoHeader[0]=(width & 0x00FF);
	infoHeader[1]=(width & 0xFF00) >> 8;
	infoHeader[2]=(height & 0x00FF);
	infoHeader[3]=(height & 0xFF00) >> 8;
	infoHeader[4]=32;
	infoHeader[5]=0;

	//save info header
	fwrite(infoHeader, 1, sizeof(infoHeader), file);

	//save the image data
	fwrite(data, 1, width*height*4, file);

	fclose(file);

	errorLog.OutputSuccess("Saved Screenshot: %s", filename);
	return;
}

//Set up variables
bool DemoInit() {
	//hide cursor
	glutSetCursor(GLUT_CURSOR_NONE);

//	SetUpARB_multitexture();
	SetUpEXT_texture_env_combine();
	SetUpEXT_draw_range_elements();
	SetUpEXT_multi_draw_arrays();

//	if(!ARB_multitexture_supported ||  !EXT_texture_env_combine_supported)
//		return false;

	//read in the map name etc from config.txt
	FILE * configFile = fopen("config.txt", "rt");
	if (!configFile) {
		errorLog.OutputError("Cannot open \"config.txt\"");
		return false;
	}

	char levelName[256];
	fscanf(configFile, "Map: %s\n", levelName);
	int curveTesselation;
	fscanf(configFile, "Curve Tesselation: %d\n", &curveTesselation);
	VECTOR3D cameraPosition;
	float angleYaw, anglePitch;
	fscanf(configFile, "Camera Position: (%f %f %f)\n", &cameraPosition.x,
			&cameraPosition.y, &cameraPosition.z);
	fscanf(configFile, "Camera Orientation: %f %f\n", &angleYaw, &anglePitch);

	fclose(configFile);

	if (!bsp.Load(levelName, curveTesselation))
		return false;

	camera.Init(4.0f, cameraPosition, angleYaw, anglePitch);

	//reset timer for start
	timer.Reset();

	return true;
}

//Set up openGL
bool GLInit() {
	//set viewport
	int height = 480;


	glViewport(0, 0, 640, 480);					//reset viewport

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);					//select projection matrix
	glLoadIdentity();										//reset
	gluPerspective(75.0f, (GLfloat) 640 / (GLfloat) height, 0.1f,
			100.0f);

	//load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//other states
	//shading
	glShadeModel(GL_SMOOTH);
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b,
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

void cbSpecial(int key, int x, int y)
{
	camera.Update(key);
	RenderFrame();
}
// mouse button callback for handling rotations
void OnMouse(int button, int state, int x, int y)
{
//	float yAlpha = x - g_nWidth/2;
//	float xAlpha = -y + g_nHeight/2;

	if (state == GLUT_DOWN)
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
//			if (x > g_nWidth/2)
//				g_eMode = ROTATE_LEFT;
//			else
//				g_eMode = ROTATE_RIGHT;
			break;
		case GLUT_RIGHT_BUTTON:
//			if (y > g_nWidth/2)
//				g_eMode = ROTATE_UP;
//			else
//				g_eMode = ROTATE_DOWN;
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		}
	}
	else
	{
		fprintf(stderr, "%d %d\n", x, y);
//		g_eMode = CAMERA_IDLE;
	}
}

void OnMouseMove(int x, int y)
{
	if ((x!= 320) && (y!=240))
	{
		camera.setMousePos(x, y);
		camera.Update(0);
		RenderFrame();
	}
}

//Perform per frame updates
void UpdateFrame(unsigned char key,  int x, int y) {
	camera.Update((int)key);
	switch (key)
	{
	//Toggle render method
	case 116:
	case 84: //T
		renderMethod = SHOW_TEXTURES;
		break;
	case 112:
	case 80: //P
		//Pause/Unpause PVS updates
		updatePVS = false;
		break;
	case 117: // U
	case 85:
		updatePVS = true;
		break;
	case 108: //L
	case 76:
		renderMethod = SHOW_LIGHTMAPS;
		break;
	case 109: //L
	case 77:
		renderMethod = MODULATE_TEXTURES;
		break;
	}
	RenderFrame();
}

//draw a frame
void RenderFrame() {
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									//reset modelview matrix

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

	if (renderMethod == MODULATE_TEXTURES)//Then modulate by lightmap, then double
			{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);

		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);

		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2.0f);
	}

	if (renderMethod == SHOW_TEXTURES)	//Then replace with previous
			{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);

		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	}

	if (renderMethod == SHOW_LIGHTMAPS)	//Then replace with lightmaps
			{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);

		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	}

	glActiveTextureARB(GL_TEXTURE0_ARB);

	if (updatePVS)
		bsp.CalculateVisibleFaces(camera.position, frustum);

	bsp.Draw();

	glPopAttrib();

	fpsCounter.Update();					//update frames per second counter
	//window.StartTextMode();
//	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	//window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());		//print the fps
//	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
//	if (renderMethod == SHOW_TEXTURES)
//		window.Print(0, 48, "Showing Textures");
//	if (renderMethod == SHOW_LIGHTMAPS)
//		window.Print(0, 48, "Showing Lightmaps");
//	if (renderMethod == MODULATE_TEXTURES)
//		window.Print(0, 48, "Showing Lit Textures");
//	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
//	if (!updatePVS)
//		window.Print(0, 68, "PVS/Frustum Cull paused");
//	window.EndTextMode();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

//	if(window.isKeyPressed(VK_F1))
//	{
//		window.SaveScreenshot();
//		window.SetKeyReleased(VK_F1);
//	}

	// All done drawing.  Let's show it.
	glutSwapBuffers();									//swap buffers

	//check for any opengl errors
//	window.CheckGLError();
}

void DemoShutdown() {
}

int main(int argc, char **argv) {
	View * v = new View(&argc, argv);
	v->createWindow("q3bsp", 640, 480);
	//Initiation
	errorLog.Init("Error Log.txt");

	if (glewInit() != GLEW_OK) {
		printf("Error in glewInit\n");

		return 1;
	}

	//init variables etc, then GL
	if (!DemoInit()) {
		errorLog.OutputError("Demo Initiation failed");
		return 0;
	} else
		errorLog.OutputSuccess("Demo Initiation Successful");

	if (!GLInit()) {
		errorLog.OutputError("OpenGL Initiation failed");
		return 0;
	} else
		errorLog.OutputSuccess("OpenGL Initiation Successful");

	// Register the callback function to do the drawing.
	v->onDraw(&RenderFrame);

	// If there's nothing to do, draw.
//	v->onIdle(&RenderFrame);
	v->onKeyboard(&UpdateFrame);
	v->setSpecialHandler(&cbSpecial);
	v->onMouse(&OnMouse);
	v->onMouseMove(&OnMouseMove);
	v->start();

	DemoShutdown();

	errorLog.OutputSuccess("Exiting...");
	return 0;								//Exit The Program
}
