//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Shadow Mapping
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
#include <GL\glut.h>
#include "LOG.h"
#include "WINDOW.h"
#include "extensions/ARB_multitexture_extension.h"
#include "extensions/EXT_blend_minmax_extension.h"
#include "extensions/EXT_texture_env_combine_extension.h"
#include "extensions/NV_register_combiners_extension.h"
#include "extensions/SGIX_depth_texture_extension.h"
#include "extensions/SGIX_shadow_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "IMAGE.h"
#include "INTERACTOR.h"
#include "main.h"
#include "SHADOW_MAP_LIGHT.h"

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

INTERACTOR camera;
SHADOW_MAP_LIGHT light;
INTERACTOR * currentInteractor=&camera;

const int numShadowMapSizes=3;
int shadowMapSizes[numShadowMapSizes]={128, 256, 512};
int shadowMapSizeIndex=1;

int shadowMapSize=shadowMapSizes[shadowMapSizeIndex];
int maxShadowMapSize=shadowMapSizes[numShadowMapSizes-1];

int depthMapPrecision=8;

GLubyte * shadowMapData8Bit;
GLuint shadowMap8Bit;
GLuint depthRamp8Bit;

GLubyte * shadowMapData16Bit;
GLuint shadowMap16Bit;
GLuint depthRamp16Bit;

GLuint shadowMap24Bit;

MATRIX4X4 bias2D(0.5f, 0.0f, 0.0f, 0.0f,	//COLUMN MAJOR
				 0.0f, 0.5f, 0.0f, 0.0f,
				 0.0f, 0.0f, 0.5f, 0.0f,
				 0.5f, 0.5f, 0.5f, 1.0f);

MATRIX4X4 bias1D(0.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, 0.0f, 0.0f, 0.0f,
				 0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, 0.0f, 0.0f, 1.0f);

MATRIX4X4 rampBias16Bit(	0.0f, 0.0f, 0.0f, 0.0f,		//puts z->s and 256*z->t
							0.0f, 0.0f, 0.0f, 0.0f,
							0.5f, 128.0f, 0.0f, 0.0f,
							0.5f, 128.0f, 0.0f, 1.0f);

COLOR ambient(0.2f, 0.2f, 0.2f, 0.2f);
COLOR dimDiffuse(0.2f, 0.2f, 0.2f, 0.2f);
COLOR brightDiffuse(1.0f, 1.0f, 1.0f, 1.0f);

bool drawFrustum=false;

enum OBJECT_TYPE{SPHERES, TORI};
OBJECT_TYPE objectType=SPHERES;

bool regcomShadowMappingSupported=false;
bool hardwareShadowMappingSupported=false;

//Set up variables
bool DemoInit()
{
	if(!window.Init("Shadow Mapping", 512, 512, 32, 24, 0, WINDOWED_SCREEN))
		return 0;											//quit if not created
	
	SetUpARB_multitexture();
	SetUpEXT_texture_env_combine();
	SetUpNV_register_combiners();
	SetUpSGIX_depth_texture();
	SetUpSGIX_shadow();
	SetUpEXT_blend_minmax();

	if(!ARB_multitexture_supported || !EXT_texture_env_combine_supported)
		return false;

	//check for register combiner shadow map support
	if(NV_register_combiners_supported)
	{
		errorLog.OutputSuccess("Register Combiner 16bit Shadow Mapping Supported");
		regcomShadowMappingSupported=true;
	}
	else
	{
		errorLog.OutputError("Register Combiner 16bit Shadow Mapping Unsupported");
		regcomShadowMappingSupported=false;
	}

	//check for hardware shadow mapping support
	if(SGIX_depth_texture_supported && SGIX_shadow_supported && EXT_blend_minmax_supported)
	{
		errorLog.OutputSuccess("Hardware Shadow Mapping Supported");
		hardwareShadowMappingSupported=true;
	}
	else
	{
		errorLog.OutputError("Hardware Shadow Mapping (SGIX_depth_texture and SGIX_shadow) Unsupported");
		errorLog.OutputError("or EXT_blend_minmax unsupported");
		hardwareShadowMappingSupported=false;
	}

	//Set up camera
	camera.Init(VECTOR3D(-2.5f, 3.5f,-2.5f));
	
	//Set Up light
	light.Init(VECTOR3D(2.0f, 3.0f, -2.0f), VECTOR3D(0.0f,-0.5f, 0.0f));
	light.SetClipDistances(2.0f, 8.0f);
	light.UpdateMatrices();



	//Create the 8 bit shadow map texture
	shadowMapData8Bit=new GLubyte[maxShadowMapSize*maxShadowMapSize];
	if(!shadowMapData8Bit)
	{
		errorLog.OutputError("Unable to allocate memory for 8 bit shadow map data");
		return false;
	}
	glGenTextures(1, &shadowMap8Bit);
	glBindTexture(GL_TEXTURE_2D, shadowMap8Bit);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_ALPHA8, shadowMapSize, shadowMapSize,
					0, GL_ALPHA, GL_UNSIGNED_BYTE, shadowMapData8Bit);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Create the 8 bit depth ramp texture
	GLubyte * depthRampData8Bit=new GLubyte[256];
	if(!depthRampData8Bit)
	{
		errorLog.OutputError("Unable to allocate memory for 8 bit depth ramp data");
		return false;
	}
	for(int i=0; i<256; i++)
		depthRampData8Bit[i]=i;

	glGenTextures(1, &depthRamp8Bit);
	glBindTexture(GL_TEXTURE_1D, depthRamp8Bit);
	glTexImage1D(	GL_TEXTURE_1D, 0, GL_ALPHA8, 256,
					0, GL_ALPHA, GL_UNSIGNED_BYTE, depthRampData8Bit);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	delete [] depthRampData8Bit;
	depthRampData8Bit=NULL;


	//Create the 16 bit shadow map texture
	if(regcomShadowMappingSupported)
	{
		shadowMapData16Bit=new GLubyte[maxShadowMapSize*maxShadowMapSize*2];
		if(!shadowMapData16Bit)
		{
			errorLog.OutputError("Unable to allocate memory for 16 bit shadow map data");
			return false;
		}
		glGenTextures(1, &shadowMap16Bit);
		glBindTexture(GL_TEXTURE_2D, shadowMap16Bit);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE8_ALPHA8, shadowMapSize, shadowMapSize,
						0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, shadowMapData16Bit);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
		//Create the 16 bit depth ramp texture
		GLubyte * depthRampData16Bit=new GLubyte[65536*2];
		if(!depthRampData16Bit)
		{
			errorLog.OutputError("Unable to allocate memory for 16 bit depth ramp data");
			return false;
		}
	
		for(int i=0; i<256; i++)
		{
			for(int j=0; j<256; j++)
			{
				depthRampData16Bit[i*512+j*2]=i;
				depthRampData16Bit[i*512+j*2+1]=j;
			}
		}

		glGenTextures(1, &depthRamp16Bit);
		glBindTexture(GL_TEXTURE_2D, depthRamp16Bit);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE8_ALPHA8, 256, 256,
						0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, depthRampData16Bit);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		delete [] depthRampData16Bit;
		depthRampData16Bit=NULL;
	}

	//Create the 24 bit shadow map texture
	if(hardwareShadowMappingSupported)
	{
		glGenTextures(1, &shadowMap24Bit);
		glBindTexture(GL_TEXTURE_2D, shadowMap24Bit);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize,
						0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

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
	gluPerspective(45.0f, (GLfloat)window.width/(GLfloat)height, 1.0f, 500.0f);
	
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

	//Set up Materials
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	return true;
}

//Perform per frame updates
void UpdateFrame()
{
	window.Update();

	currentInteractor->Update();

	//Change shadow map precision
	if(window.isKeyPressed('1') && depthMapPrecision!=8)
	{
		depthMapPrecision=8;
	}

	if(window.isKeyPressed('2') && depthMapPrecision!=16 && regcomShadowMappingSupported)
	{
		depthMapPrecision=16;
	}
	
	if(window.isKeyPressed('3') && depthMapPrecision!=24 && hardwareShadowMappingSupported)
	{
		depthMapPrecision=24;
	}

	//Increase shadow map size
	if(window.isKeyPressed(VK_UP) && shadowMapSizeIndex<numShadowMapSizes-1)
	{
		++shadowMapSizeIndex;
		shadowMapSize=shadowMapSizes[shadowMapSizeIndex];

		//recreate shadow map textures
		glBindTexture(GL_TEXTURE_2D, shadowMap8Bit);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_ALPHA8, shadowMapSize, shadowMapSize,
						0, GL_ALPHA, GL_UNSIGNED_BYTE, shadowMapData8Bit);

		if(regcomShadowMappingSupported)
		{
			glBindTexture(GL_TEXTURE_2D, shadowMap16Bit);
			glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE8_ALPHA8, shadowMapSize, shadowMapSize,
							0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, shadowMapData16Bit);
		}

		if(hardwareShadowMappingSupported)
		{
			glBindTexture(GL_TEXTURE_2D, shadowMap24Bit);
			glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize,
							0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		}
	
		window.SetKeyReleased(VK_UP);
	}

	//Decrease shadow map size
	if(window.isKeyPressed(VK_DOWN) && shadowMapSizeIndex>0)
	{
		--shadowMapSizeIndex;
		shadowMapSize=shadowMapSizes[shadowMapSizeIndex];

		//recreate shadow map textures
		glBindTexture(GL_TEXTURE_2D, shadowMap8Bit);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_ALPHA8, shadowMapSize, shadowMapSize,
						0, GL_ALPHA, GL_UNSIGNED_BYTE, shadowMapData8Bit);

		if(regcomShadowMappingSupported)
		{
			glBindTexture(GL_TEXTURE_2D, shadowMap16Bit);
			glTexImage2D(	GL_TEXTURE_2D, 0, GL_LUMINANCE8_ALPHA8, shadowMapSize, shadowMapSize,
							0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, shadowMapData16Bit);
		}

		if(hardwareShadowMappingSupported)
		{
			glBindTexture(GL_TEXTURE_2D, shadowMap24Bit);
			glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize,
							0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		}
	
		window.SetKeyReleased(VK_DOWN);
	}

	//Move the light's near clip plane
	if(window.isKeyPressed(VK_LEFT))
	{
		light.GrowFrustum();
		light.UpdateMatrices();
	}

	if(window.isKeyPressed(VK_RIGHT))
	{
		light.ShrinkFrustum();
		light.UpdateMatrices();
	}

	//toggle moving camera or light
	if(window.isKeyPressed('C'))
		currentInteractor=&camera;

	if(window.isKeyPressed('L'))
		currentInteractor=&light;

	//Toggle drawing of frustum
	if(window.isKeyPressed(' '))
	{
		drawFrustum=!drawFrustum;
		window.SetKeyReleased(' ');
	}

	//toggle spheres/ tori
	if(window.isKeyPressed('T'))
		objectType=TORI;
	if(window.isKeyPressed('B'))
		objectType=SPHERES;
}

//draw a frame
void RenderFrame()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//Draw from the light's point of view
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(light.projectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(light.viewMatrix);

	glColorMask(0, 0, 0, 0);

	if(depthMapPrecision==8 || depthMapPrecision==16)
	{
		glPolygonOffset(1.1f, 4.0f);
		//only draw back faces into shadow map
		//for some reason, just using polygon offset doesnt work
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
	}

	if(depthMapPrecision==24)
		glPolygonOffset(8.0f, 1.1f);

	glEnable(GL_POLYGON_OFFSET_FILL);

	DrawScene(true);

	glPopAttrib();



	if(depthMapPrecision==8)
	{
		//Read back the depth values into the shadow map texture image data
		glReadPixels(	0, 0, shadowMapSize, shadowMapSize, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE,
						shadowMapData8Bit);
	
		//Update shadow map
		glBindTexture(GL_TEXTURE_2D, shadowMap8Bit);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, shadowMapSize, shadowMapSize,
						GL_ALPHA, GL_UNSIGNED_BYTE, shadowMapData8Bit);
	}

	if(depthMapPrecision==16)
	{
		//Read back the depth values into the shadow map texture image data
		glReadPixels(	0, 0, shadowMapSize, shadowMapSize, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT,
						shadowMapData16Bit);
	
		//Update shadow map
		glBindTexture(GL_TEXTURE_2D, shadowMap16Bit);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, shadowMapSize, shadowMapSize,
						GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, shadowMapData16Bit);
	}

	if(depthMapPrecision==24)
	{
		glBindTexture(GL_TEXTURE_2D, shadowMap24Bit);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);
	}
	
	
	//Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//reset viewport
	glViewport(0, 0, window.width, window.height);

	//Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	//Draw from camera's point of view
	glMatrixMode(GL_MODELVIEW);
	camera.SetupViewMatrix();
	glLoadMatrixf(camera.viewMatrix);
	

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	//Draw ALL shadowed
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(light.position));
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dimDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, black);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	DrawScene(false);

	glPopAttrib();


	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	//Draw Unshadowed parts
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(light.position));
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, brightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	if(depthMapPrecision==8)
		Draw8BitUnshadowed();

	if(depthMapPrecision==16)
		Draw16BitUnshadowed();

	if(depthMapPrecision==24)
		Draw24BitUnshadowed();

	glPopAttrib();	

	//Draw the light in place
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(light.position.x, light.position.y, light.position.z);
	glutSolidSphere(0.05, 12, 12);
	glPopMatrix();
	
	//Draw the light's frustum
	if(drawFrustum)
	{
		glPushMatrix();
		MATRIX4X4 frustumMatrix=(light.projectionMatrix*light.viewMatrix).GetInverse();
		glMultMatrixf(frustumMatrix);
		glutWireCube(2.0f);
		glPopMatrix();
	}
	
	fpsCounter.Update();											//update frames per second counter
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	window.StartTextMode();
	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	window.Print(0, window.height-50, "Shadow Map Size: %d", shadowMapSize);
	glColor4f(1.0f, 0.3f, 0.3f, 1.0f);
	window.Print(0, window.height-30, "Clip Plane Distances: n:%.3f f:%.3f", light.n, light.f);
	glColor4f(0.2f, 0.5f, 0.1f, 1.0f);
	window.Print(0, window.height-10, "Shadow Map Precision: %d bit", depthMapPrecision);
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

void Draw8BitUnshadowed(void)
{
	//Set up texture units
	//Unit 0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shadowMap8Bit);
	
	//Set up tex coord generation - s, t, q coords required
	MATRIX4X4 textureProjectionMatrix2D=bias2D*light.projectionMatrix*light.viewMatrix;
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);
	
	//Unit 1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, depthRamp8Bit);

	//Set up tex coord generation - s, q coords required
	MATRIX4X4 textureProjectionMatrix1D=bias1D*light.projectionMatrix*light.viewMatrix;
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureProjectionMatrix1D.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureProjectionMatrix1D.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	
	


	//Set up texture combining
	//unit 0
	//alpha=texture alpha
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_EXT, GL_SRC_ALPHA);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
	//color=primary color
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PRIMARY_COLOR_EXT);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
		
	//unit 1
	//alpha =previous (add signed) (1-texture)
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PREVIOUS_EXT);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_EXT, GL_SRC_ALPHA);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_ADD_SIGNED_EXT);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_EXT, GL_ONE_MINUS_SRC_ALPHA);
	//color=primary color
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);



	//Use alpha test to reject shadowed fragments
	glAlphaFunc(GL_GEQUAL, 0.5f);
	glEnable(GL_ALPHA_TEST);

	DrawScene(false);
}

void Draw16BitUnshadowed(void)
{
	//Set up texture units
	//Unit 0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shadowMap16Bit);
	
	//Set up tex coord generation - s, t, q coords required
	MATRIX4X4 textureProjectionMatrix2D=bias2D*light.projectionMatrix*light.viewMatrix;
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);
	
	//Unit 1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, depthRamp16Bit);

	//Set up tex coord generation - s, t, q coords required
	MATRIX4X4 textureProjectionMatrix1D=rampBias16Bit*light.projectionMatrix*light.viewMatrix;
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureProjectionMatrix1D.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureProjectionMatrix1D.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureProjectionMatrix1D.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	
	glActiveTextureARB(GL_TEXTURE0_ARB);

	glPushAttrib(GL_ALL_ATTRIB_BITS);




	//Set up register combiners
	//1 general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//combiner 1 does spare0.a=tex0.a-tex1.a
	//disregard rgb postion of general combiner
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_DISCARD_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//A=1
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_A_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	//B=tex0.a
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_B_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	//C=1
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_C_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	//D=-tex1.a
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_D_NV, GL_TEXTURE1_ARB,
						GL_SIGNED_NEGATE_NV, GL_ALPHA);

	glCombinerOutputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//final combiner outputs	rgb=primary color rgb
	//							alpha=max(0, spare0.alpha)=max(0, tex0.a-tex1.a)
	glFinalCombinerInputNV(	GL_VARIABLE_A_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_B_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_D_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	glFinalCombinerInputNV(	GL_VARIABLE_G_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);

	glEnable(GL_REGISTER_COMBINERS_NV);


	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_ALPHA_TEST);

	DrawScene(false);

	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);




	//configure register combiners
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 2);

	//combiner 0 does spare0.rgb = tex0.rgb - tex1.rgb
	//A=1
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_RGB);
	//B=tex0.rgb
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	//C=1
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_C_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_RGB);
	//D=-tex1.rgb
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_D_NV, GL_TEXTURE1_ARB,
						GL_SIGNED_NEGATE_NV, GL_RGB);

	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//combiner 0 does spare0.a = tex1.a + (1-tex0.a) - 0.5 = tex1.a-tex0.a+0.5
	//A=1
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_A_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	//B=tex1.a
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_B_NV, GL_TEXTURE1_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	//C=1
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_C_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	//D=1-tex0.a
	glCombinerInputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_VARIABLE_D_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);

	glCombinerOutputNV(	GL_COMBINER0_NV, GL_ALPHA, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_BIAS_BY_NEGATIVE_ONE_HALF_NV, GL_FALSE, GL_FALSE, GL_FALSE);

	//combiner 1 does no rgb
	glCombinerOutputNV(	GL_COMBINER1_NV, GL_RGB, GL_DISCARD_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//combiner 1 does spare0.a = spare0.a>0.5 ? 0 : tex0.b-tex1.b
	//A=1
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_A_NV, GL_ZERO,
						GL_UNSIGNED_INVERT_NV, GL_ALPHA);
	//B=spare0.b = tex0.b - tex1.b
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_B_NV, GL_SPARE0_NV,
						GL_SIGNED_IDENTITY_NV, GL_BLUE);
	//C=0
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_C_NV, GL_ZERO,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	//D=0
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_D_NV, GL_ZERO,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);

	glCombinerOutputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_DISCARD_NV, GL_DISCARD_NV, GL_SPARE0_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_TRUE);

	//final combiner outputs	rgb=primary color rgb
	//							alpha=max(0, spare0.alpha)
	glFinalCombinerInputNV(	GL_VARIABLE_A_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_B_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(	GL_VARIABLE_D_NV, GL_PRIMARY_COLOR_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	glFinalCombinerInputNV(	GL_VARIABLE_G_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);

	glEnable(GL_REGISTER_COMBINERS_NV);

	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_ALPHA_TEST);

	DrawScene(false);
	
	glPopAttrib();
}

void Draw24BitUnshadowed()
{
	//Draw Unshadowed parts - use MAX blending to keep shadowed color in shadowed areas
	glLightfv(GL_LIGHT1, GL_DIFFUSE, brightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);

	//Set up texture units
	//Unit 0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shadowMap24Bit);
	
	//Set up tex coord generation - all 4 coordinates required
	MATRIX4X4 textureProjectionMatrix2D=bias2D*light.projectionMatrix*light.viewMatrix;
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureProjectionMatrix2D.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);
	
	//Use SGIX_shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, true);

	glBlendEquationEXT(GL_MAX_EXT);
	glEnable(GL_BLEND);
	DrawScene(false);
}

void DrawScene(bool updateAngle)
{
	static GLuint torusList, spheresList, smallTorusList;
	static float angle;	

	if(updateAngle)
		angle=timer.GetTime()/20;

	if(!torusList)
	{
		torusList=glGenLists(1);
		glNewList(torusList, GL_COMPILE);
		{
			glColor3f(0.0f, 0.0f, 1.0f);
			
			//Draw Floor
			glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
			float i;
			//Top
			glNormal3f(0.0f, 1.0f, 0.0f);
			for(i=-1.5f; i<1.5f; i+=0.2f)
			{
				glBegin(GL_TRIANGLE_STRIP);
				{
					for(float j=-1.5f; j<1.7f; j+=0.2f)
					{
						glVertex3f(j, 0.1f, i);
						glVertex3f(j, 0.1f, i+0.2f);
					}
				}
				glEnd();
			}
			
			//bottom
			glNormal3f(0.0f,-1.0f, 0.0f);
			for(i=-1.5f; i<1.5f; i+=0.2f)
			{
				glBegin(GL_TRIANGLE_STRIP);
				{
					for(float j=-1.5f; j<1.7f; j+=0.2f)
					{
						glVertex3f(j, 0.0f, i+0.2f);
						glVertex3f(j, 0.0f, i);
					}
				}
				glEnd();
			}

			//+x side
			glNormal3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			{
				for(float j=-1.5f; j<1.7f; j+=0.2f)
				{
					glVertex3f(1.5f, 0.0f, j);
					glVertex3f(1.5f, 0.1f, j);
				}
			}
			glEnd();

			//-x side
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			{
				for(float j=-1.5f; j<1.7f; j+=0.2f)
				{
					glVertex3f(-1.5f, 0.1f, j);
					glVertex3f(-1.5f, 0.0f, j);
				}
			}
			glEnd();

			//+z side
			glNormal3f(0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLE_STRIP);
			{
				for(float j=-1.5f; j<1.7f; j+=0.2f)
				{
					glVertex3f(j, 0.1f, 1.5f);
					glVertex3f(j, 0.0f, 1.5f);
				}
			}
			glEnd();

			//-z side
			glNormal3f(0.0f, 0.0f,-1.0f);
			glBegin(GL_TRIANGLE_STRIP);
			{
				for(float j=-1.5f; j<1.7f; j+=0.2f)
				{
					glVertex3f(j, 0.0f, -1.5f);
					glVertex3f(j, 0.1f, -1.5f);
				}
			}
			glEnd();
			glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);


			
			glColor3f(1.0f, 0.0f, 0.0f);
			glPushMatrix();
			glTranslatef(0.0f, 0.5f, 0.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glutSolidTorus(0.2, 0.5, 12, 24);
			glPopMatrix();
		}
		glEndList();
	}

	glCallList(torusList);

	glPushMatrix();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	if(objectType==SPHERES)
	{
		if(!spheresList)
		{
			spheresList=glGenLists(1);
			glNewList(spheresList, GL_COMPILE);
			{
				glColor3f(0.0f, 1.0f, 0.0f);
				glTranslatef(0.45f, 1.0f, 0.45f);
				glutSolidSphere(0.2, 24, 24);
				glTranslatef(-0.9f, 0.0f, 0.0f);
				glutSolidSphere(0.2, 24, 24);
				glTranslatef(0.0f, 0.0f, -0.9f);
				glutSolidSphere(0.2, 24, 24);
				glTranslatef(0.9f, 0.0f, 0.0f);
				glutSolidSphere(0.2, 24, 24);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}
			glEndList();
		}
	
		glCallList(spheresList);
	}

	if(objectType==TORI)
	{
		if(!smallTorusList)
		{
			smallTorusList=glGenLists(1);
			glNewList(smallTorusList, GL_COMPILE);
				glutSolidTorus(0.08f, 0.2f, 24, 24);
			glEndList();
		}

		glColor3f(0.0f, 1.0f, 0.0f);
		glPushMatrix();
		glTranslatef(0.45f, 1.0f, 0.45f);
		glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
		glCallList(smallTorusList);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.45f, 1.0f, 0.45f);
		glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
		glCallList(smallTorusList);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-0.45f, 1.0f,-0.45f);
		glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
		glCallList(smallTorusList);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.45f, 1.0f,-0.45f);
		glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
		glCallList(smallTorusList);
		glPopMatrix();
	}

	glPopMatrix();
}

void DemoShutdown()
{
	//Free shadow map memory
	if(shadowMapData8Bit)
		delete [] shadowMapData8Bit;
	shadowMapData8Bit=NULL;

	if(shadowMapData16Bit)
		delete [] shadowMapData16Bit;
	shadowMapData16Bit=NULL;
	
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