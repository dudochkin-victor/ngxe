//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	GPU fire
//	Downloaded from: www.paulsprojects.net
//	Created:	12th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <time.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glext.h>
#include <GL\wglext.h>
#include "LOG.h"
#include "WINDOW.h"
#include "extensions/ARB_multitexture_extension.h"
#include "extensions/ARB_texture_border_clamp_extension.h"
#include "extensions/EXT_blend_minmax_extension.h"
#include "extensions/EXT_blend_subtract_extension.h"
#include "extensions/NV_register_combiners_extension.h"
#include "extensions/WGL_ARB_extensions_string_extension.h"
#include "extensions/WGL_ARB_pixel_format_extension.h"
#include "extensions/WGL_ARB_pbuffer_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "PBUFFER.h"
#include "main.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

//errorLog MUST be kept - it is used by other files
LOG errorLog;
WINDOW window;
FPS_COUNTER fpsCounter;
TIMER timer;

COLOR windowBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);
COLOR pbufferBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

//pbuffer and texture
const int pbufferWidth=64;
const int pbufferHeight=32;
PBUFFER pbuffer;
GLuint pbufferTexture;
float texelWidth=1.0f/(float)pbufferWidth;
float texelHeight=1.0f/(float)pbufferHeight;

//how many milliseconds between texture updates?
double textureUpdateTime=50.0;

//cooling textures
const int maxCoolingValue=8;
const int numCoolingTextures=4;
GLuint coolingTextures[numCoolingTextures];

//store fuel values for bottom row
static int fuelSize=4;
static GLubyte * fuel;

//Set up variables
bool DemoInit()
{
	if(!window.Init("GPU Fire", 320, 240, 32, 24, 8, WINDOWED_SCREEN))
		return 0;											//quit if not created

	//Set up extensions
	if(	!SetUpARB_multitexture()			|| !SetUpEXT_blend_minmax()			||
		!SetUpEXT_blend_subtract()			|| !SetUpARB_texture_border_clamp()	||
		!SetUpNV_register_combiners()		|| !SetUpWGL_ARB_extensions_string())
		return false;

	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnits);
	if(maxTextureUnits<4)
	{
		errorLog.OutputError("I require at least 4 texture units");
		return false;
	}
	
	GLint maxGeneralCombiners;
	glGetIntegerv(GL_MAX_GENERAL_COMBINERS_NV, &maxGeneralCombiners);
	if(maxGeneralCombiners<3)
	{
		errorLog.OutputError("I require at least 3 general register combiners");
		return false;
	}

	//Get the WGL extensions string
	const char * wglExtensions;
	wglExtensions=wglGetExtensionsStringARB(window.hDC);

	//Set up wgl extensions
	if(!SetUpWGL_ARB_pbuffer(wglExtensions) || !SetUpWGL_ARB_pixel_format(wglExtensions))
		return false;


	//Init the pbuffer
	int pbufferIAttribs[]={0};
	int pbufferFlags[]={0};
	if(!pbuffer.Init(pbufferWidth, pbufferHeight, 32, 24, 8, 0, pbufferIAttribs, pbufferFlags))
		return false;
	
	//Share display lists and textures between the window and the pbuffer
	wglShareLists(window.hRC, pbuffer.hRC);



	//Create the pbuffer texture
	glGenTextures(1, &pbufferTexture);
	glBindTexture(GL_TEXTURE_2D, pbufferTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_ARB);

	//begin the pbuffer texture as blank
	GLubyte * initialData=new GLubyte[pbufferWidth*pbufferHeight];
	if(!initialData)
	{
		errorLog.OutputError("Unable to create space for initial texture data");
		return false;
	}
	memset(initialData, 0, pbufferWidth*pbufferHeight*sizeof(GLubyte));

	//Create texture
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_ALPHA8, pbufferWidth, pbufferHeight,
					0, GL_ALPHA, GL_UNSIGNED_BYTE, initialData);

	if(initialData)
		delete [] initialData;
	initialData=NULL;



	//Create the cooling textures
	glGenTextures(numCoolingTextures, coolingTextures);

	GLubyte * coolingData=new GLubyte[pbufferWidth*pbufferHeight];
	if(!coolingData)
	{
		errorLog.OutputError("Unable to create space for cooling texture data");
		return false;
	}

	for(int i=0; i<numCoolingTextures; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, coolingTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		for(int j=0; j<pbufferHeight; j+=2)
		{
			for(int k=0; k<pbufferWidth; k+=2)
			{
				GLubyte currentCoolingFactor=rand()%maxCoolingValue;
				coolingData[j*pbufferWidth+k]=currentCoolingFactor;
				coolingData[j*pbufferWidth+k+1]=currentCoolingFactor;
				coolingData[(j+1)*pbufferWidth+k]=currentCoolingFactor;
				coolingData[(j+1)*pbufferWidth+k+1]=currentCoolingFactor;
			}
		}

		glTexImage2D(	GL_TEXTURE_2D, 0, GL_ALPHA8, pbufferWidth, pbufferHeight,
						0, GL_ALPHA, GL_UNSIGNED_BYTE, coolingData);
	}

	if(coolingData)
		delete [] coolingData;
	coolingData=NULL;
	

	//create space for fuel
	fuel=new GLubyte[pbufferWidth];
	if(!fuel)
	{
		errorLog.OutputError("Unable to allocate memory for fuel");
		return false;
	}

	//seed random number generator
	srand( (unsigned)time( NULL ) );

	//reset timer for start
	timer.Reset();
	
	return true;
}

//Set up openGL
bool GLInit()
{
	//Set up for pbuffer
	pbuffer.MakeCurrent();

	//set viewport
	glViewport(0, 0, pbufferWidth, pbufferHeight);

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);							//select projection matrix
	glLoadIdentity();										//reset
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
		
	//load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//other states
	//shading
	glShadeModel(GL_SMOOTH);
	glClearColor(	pbufferBackgroundColor.r,
					pbufferBackgroundColor.g,
					pbufferBackgroundColor.b,
					pbufferBackgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Set up register combiners to output 0.25*(tex0+tex1+tex2+tex3)
	//ALL IN ALPHA
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 3);

	glCombinerParameterfvNV(GL_CONSTANT_COLOR0_NV, white*0.25);

	//combiner 0 does const0*tex0+const0*tex1->spare0
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_A_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_B_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_C_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_D_NV, GL_TEXTURE1_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, false, false, false);

	//combiner 1 does const0*tex2+const0*tex3->spare0
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_A_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_B_NV, GL_TEXTURE2_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_C_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_D_NV, GL_TEXTURE3_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerOutputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE1_NV,
						GL_NONE, GL_NONE, false, false, false);

	//combiner 2 does spare0+spare1->spare0
	glCombinerInputNV(	GL_COMBINER2_NV, GL_ALPHA, GL_VARIABLE_A_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER2_NV, GL_ALPHA, GL_VARIABLE_B_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER2_NV, GL_ALPHA, GL_VARIABLE_C_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER2_NV, GL_ALPHA, GL_VARIABLE_D_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	glCombinerOutputNV(	GL_COMBINER2_NV, GL_ALPHA, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, false, false, false);

	//final combiner outputs spare0
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_G_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);


	//Set up for window
	window.MakeCurrent();

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
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
	
	//load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//other states
	//shading
	glShadeModel(GL_SMOOTH);
	glClearColor(	windowBackgroundColor.r,
					windowBackgroundColor.g,
					windowBackgroundColor.b,
					windowBackgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	//set up register combiners to do red*(2*tex0)+green*(2*tex0)*(2*tex0)*(2*tex0)
	//Thus, as heat increases, color goes black->red->yellow
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 3);
	float windowConstant0[]={1.0f, 0.0f, 0.0f, 0.0f};
	glCombinerParameterfvNV(GL_CONSTANT_COLOR0_NV, windowConstant0);
	float windowConstant1[]={0.0f, 1.0f, 0.0f, 0.0f};
	glCombinerParameterfvNV(GL_CONSTANT_COLOR1_NV, windowConstant1);

	//Combiner 0 does 2*tex0->spare0
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_SCALE_BY_TWO_NV, GL_NONE, false, false, false);

	//Combiner 1 does spare0*red->col0
	//				  spare0*spare0->spare1
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_A_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_B_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_C_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_D_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER1_NV, GL_RGB, GL_PRIMARY_COLOR_NV, GL_SPARE1_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, false, false, false);

	//Combiner 2 does spare1*spare0->spare1
	glCombinerInputNV(	GL_COMBINER2_NV, GL_RGB, GL_VARIABLE_A_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER2_NV, GL_RGB, GL_VARIABLE_B_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glCombinerOutputNV(	GL_COMBINER2_NV, GL_RGB, GL_SPARE1_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, false, false, false);

	//Final combiner outputs spare1*green+col0
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_SPARE1_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_CONSTANT_COLOR1_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	return true;
}

//Perform per frame updates
void UpdateFrame()
{
	window.Update();

	//Pause/unpause
	if(window.isKeyPressed('P'))
		timer.Pause();

	if(window.isKeyPressed('U'))
		timer.Unpause();
}

//draw a frame
void RenderFrame()
{
	static double lastUpdatedTime=0.0;
	double time=timer.GetTime();

	//update the fire texture if enough time has passed
	if(lastUpdatedTime==0.0 || time-lastUpdatedTime>textureUpdateTime)
	{
		lastUpdatedTime=time;

		//Draw to pbuffer
		pbuffer.MakeCurrent();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();										//reset modelview matrix

		glPushAttrib(GL_ALL_ATTRIB_BITS);
	
		//bind & enable textures - pbuffer texture to units 0-3
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glActiveTextureARB(GL_TEXTURE2_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glActiveTextureARB(GL_TEXTURE3_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glActiveTextureARB(GL_TEXTURE0_ARB);

		//enable register combiners
		glEnable(GL_REGISTER_COMBINERS_NV);

		//Draw rectangle filling screen
		//sample four values neighboring the pixel below this one
		glBegin(GL_TRIANGLE_STRIP);
		{
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,				0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f,				0.0f-2*texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f-texelWidth,	0.0f-texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f+texelWidth,	0.0f-texelHeight);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,				1.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f,				1.0f-2*texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f-texelWidth,	1.0f-texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0.0f+texelWidth,	1.0f-texelHeight);
			glVertex3f(-1.0f,  1.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f,				0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f,				0.0f-2*texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 1.0f-texelWidth,	0.0f-texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 1.0f+texelWidth,	0.0f-texelHeight);
			glVertex3f( 1.0f, -1.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f,				1.0f);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f,				1.0f-2*texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 1.0f-texelWidth,	1.0f-texelHeight);
			glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 1.0f+texelWidth,	1.0f-texelHeight);
			glVertex3f( 1.0f,  1.0f, 0.0f);
		}
		glEnd();

		glPopAttrib();


		//subtract the cooling texture
		static int currentCoolingTexture=0;
		++currentCoolingTexture;
		if(currentCoolingTexture==numCoolingTextures)
			currentCoolingTexture=0;

		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, coolingTextures[currentCoolingTexture]);
		
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT);
		glEnable(GL_BLEND);
		
		glBegin(GL_TRIANGLE_STRIP);
		{
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,	0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,	1.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f,	0.0f);
			glVertex3f( 1.0f, -1.0f, 0.0f);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f,	1.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);
		}
		glEnd();

		glPopAttrib();




		//fill in bottom row with "fuel"
		for(int i=0; i<pbufferWidth/fuelSize; ++i)
		{
			GLubyte currentFuel=rand()%256;
			for(int j=0; j<fuelSize; ++j)
			{
				fuel[fuelSize*i+j]=currentFuel;
			}
		}

		glRasterPos2f(-1.0f, -1.0f);
		glDrawPixels(pbufferWidth, 1, GL_ALPHA, GL_UNSIGNED_BYTE, fuel);
	
		//Copy the pbuffer into the pbuffer texture
		glBindTexture(GL_TEXTURE_2D, pbufferTexture);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, pbufferWidth, pbufferHeight);
	}


	//Draw to window
	window.MakeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushAttrib(GL_ENABLE_BIT);
	
	//bind & enable textures
	//unit 0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pbufferTexture);

	//enable register combiners
	glEnable(GL_REGISTER_COMBINERS_NV);

	//Draw rectangle filling screen - don't draw bottom 3 rows
	glBegin(GL_TRIANGLE_STRIP);
	{
		glTexCoord2f(0.0f, 3*texelHeight);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, 0.0f);
		glTexCoord2f(1.0f, 3*texelHeight);
		glVertex3f( 1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, 0.0f);
	}
	glEnd();

	glPopAttrib();

	



	fpsCounter.Update();											//update frames per second counter
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	window.StartTextMode();
	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
	window.EndTextMode();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if(window.isKeyPressed(VK_F1))
	{
		window.SaveScreenshot();
		window.SetKeyReleased(VK_F1);
	}

	window.SwapBuffers();									//swap buffers

	//check for any opengl errors
	window.CheckGLError();

	//quit if necessary
	if(window.isKeyPressed(VK_ESCAPE))
		PostQuitMessage(0);
}

void DemoShutdown()
{
	pbuffer.Shutdown();

	window.Shutdown();										//Shutdown window
}

//ENTRY POINT FOR APPLICATION
//CALL WINDOW CREATION ROUTINE, DEAL WITH MESSAGES, WATCH FOR INTERACTION
int WINAPI WinMain(	HINSTANCE	hInstance,				//instance
					HINSTANCE	hPrevInstance,			//Previous Instance
					LPSTR		lpCmdLine,				//command line parameters
					int			nCmdShow)				//Window show state
{
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

	//Main Loop
	for(;;)
	{
		if(!(window.HandleMessages())) break;//handle windows messages, quit if returns false
		UpdateFrame();
		RenderFrame();
	}

	DemoShutdown();
	
	errorLog.OutputSuccess("Exiting...");
	return (window.msg.wParam);								//Exit The Program
}