//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Refraction
//	Downloaded from: www.paulsprojects.net
//	Created:	1st September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glut.h>
#include <GL\glext.h>
#include <GL\wglext.h>
#include "LOG.h"
#include "WINDOW.h"
#include "extensions/ARB_multitexture_extension.h"
#include "extensions/ARB_texture_cube_map_extension.h"
#include "extensions/EXT_texture_edge_clamp_extension.h"
#include "extensions/NV_register_combiners_extension.h"
#include "extensions/NV_register_combiners2_extension.h"
#include "extensions/NV_vertex_program_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "INTERACTOR.h"
#include "IMAGE.h"
#include "NV_vertex_program.h"
#include "main.h"

//link to libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "winmm.lib")

//errorLog MUST be kept - it is used by other files
LOG errorLog;
WINDOW window;
FPS_COUNTER fpsCounter;
TIMER timer;

COLOR backgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

INTERACTOR camera;


//Cube map
IMAGE	cubeMapPosX, cubeMapNegX,
		cubeMapPosY, cubeMapNegY,
		cubeMapPosZ, cubeMapNegZ;
GLuint cubeMapTexture;

//vertex programs
GLuint chromaticSingleVP;
GLuint simpleSingleVP;
GLuint chromaticTwin1VP, chromaticTwin2VP;

//refractive index
GLfloat index=1.2f;
GLfloat indexDelta=0.03f;

//fresnel multiplication factor
float fresnel=2.0f;

//Object type
GLuint objectType=0;

//Path
enum RENDER_PATH{CHROMATIC_SINGLE, SIMPLE_SINGLE, CHROMATIC_TWIN};
RENDER_PATH renderPath=CHROMATIC_TWIN;
bool pathOneSupported=false;

//Set up variables
bool DemoInit()
{
	if(!window.Init("Project Template", 640, 480, 32, 24, 8, WINDOWED_SCREEN))
		return 0;											//quit if not created

	SetUpARB_multitexture();
	SetUpARB_texture_cube_map();
	SetUpEXT_texture_edge_clamp();
	SetUpNV_register_combiners();
	SetUpNV_register_combiners2();
	SetUpNV_vertex_program();

	//Check for necessary extensions
	if(	!ARB_multitexture_supported || !ARB_texture_cube_map_supported ||
		!EXT_texture_edge_clamp_supported || !NV_register_combiners_supported ||
		!NV_vertex_program_supported)
		return false;

	//Check for single-pass chromatic aberration states
	GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnits);
	if( NV_register_combiners2_supported && maxTextureUnits>=4)
	{
		errorLog.OutputSuccess("Single Pass Chromatic Aberration Supported!");
		pathOneSupported=true;
		renderPath=CHROMATIC_SINGLE;
	}

	camera.Init(VECTOR3D(0.0f, 0.0f, 4.0f), 2.5f, 10.0f);

	if(	!cubeMapPosX.Load("cube_face_posx.tga") ||
		!cubeMapNegX.Load("cube_face_negx.tga") ||
		!cubeMapPosY.Load("cube_face_posy.tga") ||
		!cubeMapNegY.Load("cube_face_negy.tga") ||
		!cubeMapPosZ.Load("cube_face_posz.tga") ||
		!cubeMapNegZ.Load("cube_face_negz.tga"))
		return false;

	//Build a texture from the data
	glGenTextures(1, &cubeMapTexture);								//Generate Texture ID
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);					//Bind texture
	
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
					0, GL_RGBA8, cubeMapPosX.width, cubeMapPosX.height, 0,
					cubeMapPosX.format, GL_UNSIGNED_BYTE, cubeMapPosX.data);
	
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
					0, GL_RGBA8, cubeMapNegX.width, cubeMapNegX.height, 0,
					cubeMapNegX.format, GL_UNSIGNED_BYTE, cubeMapNegX.data);

	glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
					0, GL_RGBA8, cubeMapPosY.width, cubeMapPosY.height, 0,
					cubeMapPosY.format, GL_UNSIGNED_BYTE, cubeMapPosY.data);
	
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
					0, GL_RGBA8, cubeMapNegY.width, cubeMapNegY.height, 0,
					cubeMapNegY.format, GL_UNSIGNED_BYTE, cubeMapNegY.data);

	glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
					0, GL_RGBA8, cubeMapPosZ.width, cubeMapPosZ.height, 0,
					cubeMapPosZ.format, GL_UNSIGNED_BYTE, cubeMapPosZ.data);
	
	glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
					0, GL_RGBA8, cubeMapNegZ.width, cubeMapNegZ.height, 0,
					cubeMapNegZ.format, GL_UNSIGNED_BYTE, cubeMapNegZ.data);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
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
	gluPerspective(45.0f, (GLfloat)window.width/(GLfloat)height, 0.1f, 100.0f);
	
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


	//load vertex programs
	glGenProgramsNV(1, &chromaticSingleVP);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, chromaticSingleVP);
	LoadNV_vertex_program("chromatic single vp.txt", chromaticSingleVP);

	glGenProgramsNV(1, &simpleSingleVP);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, simpleSingleVP);
	LoadNV_vertex_program("simple single vp.txt", simpleSingleVP);

	glGenProgramsNV(1, &chromaticTwin1VP);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, chromaticTwin1VP);
	LoadNV_vertex_program("chromatic twin 1 vp.txt", chromaticTwin1VP);

	glGenProgramsNV(1, &chromaticTwin2VP);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, chromaticTwin2VP);
	LoadNV_vertex_program("chromatic twin 2 vp.txt", chromaticTwin2VP);




	//Set track matrices
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4, GL_MODELVIEW, GL_IDENTITY_NV);
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 8, GL_MODELVIEW, GL_INVERSE_TRANSPOSE_NV);
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 12, GL_TEXTURE, GL_IDENTITY_NV);

	//Send fresnel multipication factor to c[28]
	glProgramParameter4fNV(	GL_VERTEX_PROGRAM_NV, 28, fresnel, fresnel, fresnel, 1.0f);
	
	//Send refractive indices to c[32], c[33], c[34]
	glProgramParameter4fNV(	GL_VERTEX_PROGRAM_NV, 32, index, index*index, 0.0f, 1.0f);
	glProgramParameter4fNV(	GL_VERTEX_PROGRAM_NV, 33, index+indexDelta,
							(index+indexDelta)*(index+indexDelta), 0.0f, 1.0f);
	glProgramParameter4fNV(	GL_VERTEX_PROGRAM_NV, 34, index+2*indexDelta,
							(index+2*indexDelta)*(index+2*indexDelta), 0.0f, 1.0f);
	//Send useful constants to c[64]
	glProgramParameter4fNV(	GL_VERTEX_PROGRAM_NV, 64, 0.0f, 1.0f, 2.0f, 0.5f);



	//Init states to begin
	if(renderPath==CHROMATIC_SINGLE)
		InitChromaticSingleStates();

	if(renderPath==SIMPLE_SINGLE)
		InitSimpleSingleStates();
	
	return true;
}


void InitChromaticSingleStates()
{
	//Set up the register combiners
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 2);

	//Enable per stage constants
	glEnable(GL_PER_STAGE_CONSTANTS_NV);

	//Send (1, 0, 0, 0) and (0, 1, 0, 0) as combiner 0 constant colors
	GLfloat combiner0Constant0[]={1.0f, 0.0f, 0.0f, 0.0f};
	glCombinerStageParameterfvNV(GL_COMBINER0_NV, GL_CONSTANT_COLOR0_NV, combiner0Constant0);
	GLfloat combiner0Constant1[]={0.0f, 1.0f, 0.0f, 0.0f};
	glCombinerStageParameterfvNV(GL_COMBINER0_NV, GL_CONSTANT_COLOR1_NV, combiner0Constant1);

	//Send (0, 0, 1, 0) as combiner 1 constant color 0
	GLfloat combiner1Constant0[]={0.0f, 0.0f, 1.0f, 0.0f};
	glCombinerStageParameterfvNV(GL_COMBINER1_NV, GL_CONSTANT_COLOR0_NV, combiner1Constant0);

	//Combiner 0 does:	red(tex2)+green(tex1)->spare0.rgb
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE2_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_C_NV, GL_TEXTURE1_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_D_NV, GL_CONSTANT_COLOR1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//Combiner 1 does:	blue(tex0)+spare0.rgb->spare0.rgb
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_B_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_C_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_D_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER1_NV, GL_RGB, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_TEXTURE3_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	//Bind vertex program
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, chromaticSingleVP);
}

void InitSimpleSingleStates()
{
	//Set up the register combiners
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 2);

	//Disable per-stage constants if we have been using them
	if(NV_register_combiners2_supported)
		glDisable(GL_PER_STAGE_CONSTANTS_NV);

	//Final combiner does col0*tex1 + (1-col0)*tex0
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_TEXTURE1_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_TEXTURE0_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	//Bind vertex program
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, simpleSingleVP);
}

void InitChromaticTwin1States()
{
	//Set up the register combiners
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//Disable per-stage constants if we have been using them
	if(NV_register_combiners2_supported)
		glDisable(GL_PER_STAGE_CONSTANTS_NV);

	//Send (0, 1, 0, 0) and (0, 0, 1, 0) as constant colors
	GLfloat constant0[]={0.0f, 1.0f, 0.0f, 0.0f};
	glCombinerParameterfvNV(GL_CONSTANT_COLOR0_NV, constant0);
	GLfloat constant1[]={0.0f, 0.0f, 1.0f, 0.0f};
	glCombinerParameterfvNV(GL_CONSTANT_COLOR1_NV, constant1);

	//Combiner 0 does:	green(tex1)+blue(tex0)->spare0.rgb
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE1_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_C_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_D_NV, GL_CONSTANT_COLOR1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	//Bind vertex program
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, chromaticTwin1VP);
}

void InitChromaticTwin2States()
{
	//Set up the register combiners
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//Disable per-stage constants if we have been using them
	if(NV_register_combiners2_supported)
		glDisable(GL_PER_STAGE_CONSTANTS_NV);

	//Send (1, 0, 0, 0) as constant color 0
	GLfloat constant0[]={1.0f, 0.0f, 0.0f, 0.0f};
	glCombinerParameterfvNV(GL_CONSTANT_COLOR0_NV, constant0);
	
	//Combiner 0 does:	red(tex0)->spare0.rgb
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_CONSTANT_COLOR0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_TEXTURE1_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	//Bind vertex program
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, chromaticTwin2VP);

	//Enable blend to add to first pass
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
}



//Perform per frame updates
void UpdateFrame()
{
	window.Update();
	camera.Update();

	//Change object type
	if(window.isKeyPressed(' '))
	{
		++objectType;
		if(objectType==3)
			objectType=0;

		window.SetKeyReleased(' ');
	}

	//Change render path
	if(window.isKeyPressed('1') && renderPath!=CHROMATIC_SINGLE && pathOneSupported)
	{
		renderPath=CHROMATIC_SINGLE;
		InitChromaticSingleStates();
	}
	
	if(window.isKeyPressed('2') && renderPath!=SIMPLE_SINGLE)
	{
		renderPath=SIMPLE_SINGLE;
		InitSimpleSingleStates();
	}

	if(window.isKeyPressed('3') && renderPath!=CHROMATIC_TWIN)
	{
		renderPath=CHROMATIC_TWIN;
		//Set States every pass, as it is a 2 pass algorithm
	}
}

//draw a frame
void RenderFrame()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix


	camera.SetupViewMatrix();
	glMultMatrixf(camera.viewMatrix);

	DrawBackground();


	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	//Enable vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);

	//Enable register combiners
	glEnable(GL_REGISTER_COMBINERS_NV);

	//Enable textures
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	//Set up CHROMATIC_SINGLE tex units
	if(renderPath==CHROMATIC_SINGLE)
	{
		glActiveTextureARB(GL_TEXTURE2_ARB);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);

		glActiveTextureARB(GL_TEXTURE3_ARB);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	}
	
	glActiveTextureARB(GL_TEXTURE0_ARB);

	//Set the texture Matrix to be the inverse view matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMultMatrixf(camera.viewMatrix.GetInverse());
	glMatrixMode(GL_MODELVIEW);

	//Create object display lists
	static GLuint objectLists;
	if(!objectLists)
	{
		objectLists=glGenLists(3);
		
		glNewList(objectLists, GL_COMPILE);
		{
			glutSolidSphere(1.0, 120, 60);
		}
		glEndList();

		glNewList(objectLists+1, GL_COMPILE);
		{
			glutSolidTorus(0.3, 0.7, 120, 60);
		}
		glEndList();

		glNewList(objectLists+2, GL_COMPILE);
		{
			glutSolidTeapot(1.0f);
		}
		glEndList();
	}

	//If using CHROMATIC_TWIN, need to do two passes, so set states every frame
	if(renderPath==CHROMATIC_TWIN)
		InitChromaticTwin1States();

	//draw object
	glCallList(objectLists+objectType);

	//Do the second path if necessary
	if(renderPath==CHROMATIC_TWIN)
	{
		InitChromaticTwin2States();
		glCallList(objectLists+objectType);
	}

	//Reset texture matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();

	


	fpsCounter.Update();											//update frames per second counter
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	window.StartTextMode();
	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
	glColor4f(0.5f, 0.0f, 1.0f, 1.0f);
	if(renderPath==CHROMATIC_SINGLE)
		window.Print(0, 48, "Single Pass Chromatic Aberration");
	if(renderPath==SIMPLE_SINGLE)
		window.Print(0, 48, "Single Pass Simple Refraction");
	if(renderPath==CHROMATIC_TWIN)
		window.Print(0, 48, "Two Pass Chromatic Aberration");
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


//Draw background
void DrawBackground()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//Enable textures
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	
	//Enable texgen
	float xPlane[]={1.0f, 0.0f, 0.0f, 0.0f};
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, xPlane);
	glEnable(GL_TEXTURE_GEN_S);
	float yPlane[]={0.0f, 1.0f, 0.0f, 0.0f};
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, yPlane);
	glEnable(GL_TEXTURE_GEN_T);
	float zPlane[]={0.0f, 0.0f, 1.0f, 0.0f};
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_R, GL_OBJECT_PLANE, zPlane);
	glEnable(GL_TEXTURE_GEN_R);
	
	//Draw environment
	static GLuint envList;
	if(!envList)
	{
		envList=glGenLists(1);
		glNewList(envList, GL_COMPILE);
		{
			glutSolidSphere(15.0f, 60, 30);
		}
		glEndList();
	}
	glCallList(envList);

	glPopAttrib();
}



void DemoShutdown()
{
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