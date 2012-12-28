//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Per Pixel Attenuation
//	Downloaded from: www.paulsprojects.net
//	Created:	25th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glext.h>
#include <GL\wglext.h>
#include "LOG.h"
#include "WINDOW.h"
#include "extensions/ARB_multitexture_extension.h"
#include "extensions/EXT_texture3D_extension.h"
#include "extensions/EXT_texture_edge_clamp_extension.h"
#include "extensions/NV_register_combiners_extension.h"
#include "extensions/NV_texture_shader_extension.h"
#include "extensions/NV_vertex_program_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "IMAGE.h"
#include "INTERACTOR.h"
#include "NV_vertex_program.h"
#include "InitLightTextures.h"
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

COLOR backgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

//surface decal texture
IMAGE decalImage;
GLuint decalTexture;

//light parameters
VECTOR3D lightPosition(0.0f, 0.0f, 0.0f);
VECTOR3D lightMovement(0.006f, 0.008f, 0.003f);
float lightRadius=1.3f;

COLOR lightColors[4];
int currentLightColor=0;

//light sphere
GLUquadricObj * sphere;

//vertex program
GLuint vertexProgram;

//attenuation textures
GLuint atten2DTexture, atten1DTexture, atten3DTexture;
GLuint gaussian2DTexture, gaussian1DTexture;

//camera
INTERACTOR camera;

enum CODE_PATH{TEXTURE_ATTEN, PASS_THROUGH_ATTEN, TEXTURE_3D_ATTEN, GAUSSIAN_ATTEN};
CODE_PATH codePath=TEXTURE_ATTEN;

//Set up variables
bool DemoInit()
{
	if(!window.Init("Project Template", 640, 480, 32, 24, 8, WINDOWED_SCREEN))
		return 0;											//quit if not created
	
	SetUpARB_multitexture();
	SetUpEXT_texture3D();
	SetUpEXT_texture_edge_clamp();
	SetUpNV_register_combiners();
	SetUpNV_texture_shader();
	SetUpNV_vertex_program();

	if(	!EXT_texture_edge_clamp_supported || !ARB_multitexture_supported ||
		!NV_vertex_program_supported || !NV_register_combiners_supported)
		return false;
	
	//Check we have at least 3 texture units
	GLint maxTextureUnitsARB;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnitsARB);

	if(maxTextureUnitsARB<3)
	{
		errorLog.OutputError("I require at least 3 texture units");
		return false;
	}

	//Set light colors
	lightColors[0].Set(1.0f, 1.0f, 1.0f, 1.0f);
	lightColors[1].Set((float)47/255, (float)206/255, (float)240/255, 1.0f);
	lightColors[2].Set((float)254/255, (float)48/255, (float)18/255, 1.0f);
	lightColors[3].Set((float)83/255, (float)243/255, (float)29/255, 1.0f);
	
	

	//Load textures
	//Decal image
	decalImage.Load("decal.tga");
	glGenTextures(1, &decalTexture);
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA8, decalImage.width, decalImage.height,
					0, decalImage.format, GL_UNSIGNED_BYTE, decalImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	//Create light textures
	if(!InitLightTextures(	atten1DTexture, atten2DTexture, atten3DTexture,
							gaussian1DTexture, gaussian2DTexture))
		return false;
	

	camera.Init(VECTOR3D(0.0f, 0.0f, 3.5f));

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
	gluPerspective(45.0f, (GLfloat)window.width/(GLfloat)height, 1.0f, 100.0f);
	
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

	//set up sphere
	sphere=gluNewQuadric();

	//Set up vertex arrays for quad
	glVertexPointer(3, GL_FLOAT, 0, vertexPositions);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_COORD_ARRAY);

	//load vertex program
	glGenProgramsNV(1, &vertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgram);
	LoadNV_vertex_program("vertex program.txt", vertexProgram);

	//Set track matrices
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4, GL_TEXTURE0_ARB, GL_IDENTITY_NV);

	//Set texture atten states
	SetTextureAttenStates();

	return true;
}


//Set the states for texture attenuation
void SetTextureAttenStates()
{
	//Set up texture units
	//Unit 0 - decal texture
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	//Unit 1 - 2D attenuation
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, atten2DTexture);
	//Unit 2 - 1D attenuation
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_1D, atten1DTexture);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	//Set up register combiners
	//one general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//combiner 0 does tex1+tex2 -> spare0
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE1_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_C_NV, GL_TEXTURE2_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_D_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//final combiner outputs (1-spare0)*tex0*col0
	//first do tex0*col0 in the EF multiplier
	glFinalCombinerInputNV(GL_VARIABLE_E_NV, GL_TEXTURE0_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_F_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	//now do rest
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_E_TIMES_F_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
}

//Set the states for pass through atten
void SetPassThroughAttenStates()
{
	//Set up texture units
	//Unit 0 - decal texture
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	
	//Set up texture shaders
	//unit 0 - decal
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_TEXTURE_2D);
	//unit 1 - pass through atten coords
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_PASS_THROUGH_NV);
	glActiveTextureARB(GL_TEXTURE0_ARB);


	//Set up register combiners
	//one general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);
	
	//combiner 0 does tex1 dot tex1 -> spare0
	//spare 0 thus holds the squared length of the vector in tex1
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE1_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_TEXTURE1_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_TRUE, GL_FALSE, GL_FALSE);

	//Set up final combiner to output clampto01(1-spare0)*col0*tex0
	//first do tex0*col0 in the EF multiplier
	glFinalCombinerInputNV(GL_VARIABLE_E_NV, GL_TEXTURE0_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_F_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	//now do rest
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_SPARE0_NV, GL_UNSIGNED_INVERT_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_E_TIMES_F_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
}

void SetTexture3DAttenStates()
{
	//Set up texture units
	//Unit 0 - decal texture
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	//Unit 1 - 3d attanuation
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_3D, atten3DTexture);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	
	//Set up register combiners
	//one general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);
	
	//Set up final combiner to output tex1*col0*tex0
	//first do tex0*col0 in the EF multiplier
	glFinalCombinerInputNV(GL_VARIABLE_E_NV, GL_TEXTURE0_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_F_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	//now do rest
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_TEXTURE1_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_E_TIMES_F_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
}

//Set the states for texture attenuation
void SetGaussianAttenStates()
{
	//Set up texture units
	//Unit 0 - decal texture
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	//Unit 1 - 2D attenuation
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, gaussian2DTexture);
	//Unit 2 - 1D attenuation
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_1D, gaussian1DTexture);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	//Set up register combiners
	//one general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//combiner 0 does tex1*tex2 -> spare0
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE1_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_TEXTURE2_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//final combiner outputs spare0*tex0*col0
	//first do tex0*col0 in the EF multiplier
	glFinalCombinerInputNV(GL_VARIABLE_E_NV, GL_TEXTURE0_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_F_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	//now do rest
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_E_TIMES_F_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
}


//Perform per frame updates
void UpdateFrame()
{
	window.Update();
	camera.Update();

	//Update the light's position
	lightPosition+=lightMovement;

	//reflect the velocity if necessary
	if(lightPosition.x>0.9f || lightPosition.x<-0.9f)
		lightMovement.x=-lightMovement.x;
	
	if(lightPosition.y>0.9f || lightPosition.y<-0.9f)
		lightMovement.y=-lightMovement.y;
	
	if(lightPosition.z>0.9f || lightPosition.z<-0.9f)
		lightMovement.z=-lightMovement.z;

	if(window.isKeyPressed('1') && codePath!=TEXTURE_ATTEN)
	{
		codePath=TEXTURE_ATTEN;
		SetTextureAttenStates();
	}

	if(	window.isKeyPressed('2') && codePath!=PASS_THROUGH_ATTEN &&
		NV_texture_shader_supported)
	{
		codePath=PASS_THROUGH_ATTEN;
		SetPassThroughAttenStates();
	}

	if(	window.isKeyPressed('3') && codePath!=TEXTURE_3D_ATTEN &&
		EXT_texture3D_supported)
	{
		codePath=TEXTURE_3D_ATTEN;
		SetTexture3DAttenStates();
	}

	if( window.isKeyPressed('4') && codePath!=GAUSSIAN_ATTEN)
	{
		codePath=GAUSSIAN_ATTEN;
		SetGaussianAttenStates();
	}

	if(	window.isKeyPressed(' '))
	{
		currentLightColor++;
		if(currentLightColor==4)
			currentLightColor=0;
	
		window.SetKeyReleased(' ');
	}
}

//draw a frame
void RenderFrame()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix

	camera.SetupViewMatrix();
	glLoadMatrixf(camera.viewMatrix);

	//Enable box texture
	glEnable(GL_TEXTURE_2D);

	//Enable attenuation textures
	if(codePath==TEXTURE_ATTEN || codePath==GAUSSIAN_ATTEN)
	{
		//2D atten
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		//1D atten
		glActiveTextureARB(GL_TEXTURE2_ARB);
		glEnable(GL_TEXTURE_1D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}
	else if(codePath==PASS_THROUGH_ATTEN)	//set up pass through states
	{
		//enable texture shaders
		glEnable(GL_TEXTURE_SHADER_NV);
	}
	else if(codePath==TEXTURE_3D_ATTEN)
	{
		//3D atten
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_3D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}
	


	//Enable vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgram);

	//Send the light's color to primary color
	glColor4fv(lightColors[currentLightColor]);
						
	//set the texture matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(0.5f, 0.5f, 0.5f);
	glScalef(1/lightRadius, 1/lightRadius, 1/lightRadius);
	glTranslatef(-lightPosition.x, -lightPosition.y, -lightPosition.z);
	glMatrixMode(GL_MODELVIEW);

	//Enable register combiners
	glEnable(GL_REGISTER_COMBINERS_NV);

	//Draw Cube
	glDrawArrays(GL_QUADS, 0, 24);

	//Disable box texture
	glDisable(GL_TEXTURE_2D);

	//Disable attenuation textures
	if(codePath==TEXTURE_ATTEN || codePath==GAUSSIAN_ATTEN)
	{
		//2D atten
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		//1D atten
		glActiveTextureARB(GL_TEXTURE2_ARB);
		glDisable(GL_TEXTURE_1D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}
	else if(codePath==PASS_THROUGH_ATTEN)	//set up pass through states
	{
		//disable texture shaders
		glDisable(GL_TEXTURE_SHADER_NV);
	}
	else if(codePath==TEXTURE_3D_ATTEN)
	{
		//3D atten
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_3D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}
	
	//Disable vertex program
	glDisable(GL_VERTEX_PROGRAM_NV);
	
	//reset the texture matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	//Disable register combiners
	glDisable(GL_REGISTER_COMBINERS_NV);


	//Draw light in a display list
	glColor4fv(lightColors[currentLightColor]);
	glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
	static GLuint sphereList=0;
	if(!sphereList)
	{
		sphereList=glGenLists(1);
		glNewList(sphereList, GL_COMPILE);
		{
			gluSphere(sphere, 0.1f, 24, 12);
		}
		glEndList();
	}
	glCallList(sphereList);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	


	fpsCounter.Update();											//update frames per second counter
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	window.StartTextMode();
	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
	if(codePath==TEXTURE_ATTEN)
		window.Print(0, 48, "Using Texture Attenuation");
	if(codePath==PASS_THROUGH_ATTEN)
		window.Print(0, 48, "Using Pass Through Attenuation");
	if(codePath==TEXTURE_3D_ATTEN)
		window.Print(0, 48, "Using 3D Texture Attenuation");
	if(codePath==GAUSSIAN_ATTEN)
		window.Print(0, 48, "Using Gaussian Texture Attenuation");
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