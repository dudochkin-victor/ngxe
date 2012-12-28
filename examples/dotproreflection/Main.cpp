//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Dot product reflect
//	Downloaded from: www.paulsprojects.net
//	Created:	26th August 2002
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
#include "extensions/ARB_texture_cube_map_extension.h"
#include "extensions/EXT_compiled_vertex_array_extension.h"
#include "extensions/EXT_texture_edge_clamp_extension.h"
#include "extensions/NV_register_combiners_extension.h"
#include "extensions/NV_texture_shader_extension.h"
#include "extensions/NV_vertex_program_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "IMAGE.h"
#include "main.h"
#include "TORUS.h"
#include "NV_vertex_program.h"

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

IMAGE bumpMapImage;
GLuint bumpMapTexture;

IMAGE	cubeMapPosX, cubeMapNegX,
		cubeMapPosY, cubeMapNegY,
		cubeMapPosZ, cubeMapNegZ;
GLuint cubeMapTexture;

TORUS torus;

GLuint vertexProgram;

//rotation angles
double angleYaw=0.0, anglePitch=30.0;

//Set up variables
bool DemoInit()
{
	if(!window.Init("Dot Product Reflect", 640, 480, 32, 24, 8, WINDOWED_SCREEN))
		return 0;											//quit if not created

	SetUpARB_multitexture();
	SetUpEXT_texture_edge_clamp();
	SetUpNV_register_combiners();
	SetUpNV_texture_shader();
	SetUpARB_texture_cube_map();
	SetUpNV_vertex_program();
	SetUpEXT_compiled_vertex_array();
	
	if(	!ARB_multitexture_supported || !EXT_texture_edge_clamp_supported ||
		!NV_register_combiners_supported || !NV_texture_shader_supported ||
		!ARB_texture_cube_map_supported || !NV_vertex_program_supported ||
		!EXT_compiled_vertex_array_supported)
		return false;

	//Load bump map, convert to signed data, and create signed texture from it
	if(!bumpMapImage.Load("normalmap.tga"))
		return false;
 
	//Create space for signed data
	GLbyte * signedData=new GLbyte[bumpMapImage.width*bumpMapImage.height*3];
	if(!signedData)
	{
		errorLog.OutputError("Unable to allocate memory for signed texture data");
		return false;
	}

	//Convert unsigned to signed RGB data, ignoring alpha
	if(bumpMapImage.bpp==24)
	{
		for(unsigned int i=0; i<bumpMapImage.width*bumpMapImage.height*3; i++)
			signedData[i]=bumpMapImage.data[i]-128;
	}
	if(bumpMapImage.bpp==32)
	{
		for(unsigned int i=0; i<bumpMapImage.width*bumpMapImage.height; i++)
		{
			for(unsigned int j=0; j<3; j++)
			{
				signedData[i*3+j]=bumpMapImage.data[i*4+j]-128;
			}
		}
	}

	glGenTextures(1, &bumpMapTexture);
	glBindTexture(GL_TEXTURE_2D, bumpMapTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_SIGNED_RGBA8_NV, bumpMapImage.width, bumpMapImage.height,
					0, GL_RGB, GL_BYTE, signedData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Load cube map and create texture
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

	//Set up register combiners to output texture3
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	glFinalCombinerInputNV(	GL_VARIABLE_A_NV, GL_TEXTURE3_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_B_NV, GL_ZERO, GL_UNSIGNED_INVERT_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);


	//Set up texture shaders to do a "Dot Product Reflect Cube Map"
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_DOT_PRODUCT_NV);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_PREVIOUS_TEXTURE_INPUT_NV, GL_TEXTURE0_ARB);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_DOT_PRODUCT_NV);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_PREVIOUS_TEXTURE_INPUT_NV, GL_TEXTURE0_ARB);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_PREVIOUS_TEXTURE_INPUT_NV, GL_TEXTURE0_ARB);

	glActiveTextureARB(GL_TEXTURE0_ARB);

	//Set up vertex program
	glGenProgramsNV(1, &vertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgram);
	LoadNV_vertex_program("vertex program.txt", vertexProgram);

	//track matrices
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4, GL_MODELVIEW, GL_IDENTITY_NV);
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 8, GL_MODELVIEW, GL_INVERSE_TRANSPOSE_NV);
	
	//Set up vertex arrays for torus
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(3, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].position);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].normal);

	//Send Texture Coordinates to unit 0
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].s);
	
	//Send Tangents to unit 1,2
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].sTangent);
	
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].tTangent);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	
	//Use CVA
	glLockArraysEXT(0, torus.numVertices);

	return true;
}

//Perform per frame updates
void UpdateFrame()
{
	window.Update();
}

//draw a frame
void RenderFrame()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix

	//Draw environment
	DrawScenery();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//Set up texture units
	//Unit 0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bumpMapTexture);

	//Unit 3
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glEnable(GL_REGISTER_COMBINERS_NV);
	glEnable(GL_TEXTURE_SHADER_NV);

	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgram);

	//rotate with the mouse
	if(window.isLeftButtonPressed())
	{
		angleYaw+=(double)window.GetMouseXMovement()/3;
		anglePitch+=(double)window.GetMouseYMovement()/3;
	}

	glTranslatef(0.0f, 0.0f, -12.0f);
	glRotated(anglePitch, 1.0f, 0.0f, 0.0f);
	glRotated(-angleYaw, 0.0f, 0.0f, 1.0f);

	glEnable(GL_CULL_FACE);

	//Draw torus
	glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);



	glPopAttrib();

	fpsCounter.Update();											//update frames per second counter
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
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

void DrawScenery()
{
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMapTexture);

	glBegin(GL_QUADS);
	{
		glTexCoord3f(	 1.0f, 1.0f,-1.0f);
		glVertex3f(		 1.0f, 1.0f,-1.0f);
		
		glTexCoord3f(	-1.0f, 1.0f,-1.0f);
		glVertex3f(		-1.0f, 1.0f,-1.0f);
		
		glTexCoord3f(	-1.0f,-1.0f,-1.0f);
		glVertex3f(		-1.0f,-1.0f,-1.0f);
		
		glTexCoord3f(	 1.0f,-1.0f,-1.0f);
		glVertex3f(		 1.0f,-1.0f,-1.0f);
	}
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glPopMatrix();
}

void DemoShutdown()
{
	glUnlockArraysEXT();
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