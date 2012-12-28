//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Shadow Volumes
//	Downloaded from: www.paulsprojects.net
//	Created:	4th September 2002
//	Updated:	3rd October 2002	-	Added EXT_stencil_two_side support
//										Added vertex program extrusion
//										Improved "zFail required" testing
//				15th November 2002	-	Corrected problem on systems not suppotring
//										ARB_vertex_program.
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
#include "extensions/EXT_stencil_two_side_extension.h"
#include "extensions/EXT_stencil_wrap_extension.h"
#include "extensions/ARB_vertex_program_extension.h"
#include "ARB_program.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "SHADOW_MODEL.h"
#include "INTERACTOR.h"
#include "main.h"
#include "box.h"

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

LIGHT light;

SHADOW_MODEL models[2];
float modelRadii[2];

const int numTori=4;
TORUS torus[numTori];

//Always use zFail?
bool alwaysUseZFail=true;

//2 sided stencil supported?
bool twoSidedStencilSupported=false;
//Using 2 sided stencil?
bool useTwoSidedStencil=false;

//Vertex program volume generation supported?
bool VPVolumesSupported=false;
//Using vertex program volume generation
bool useVPVolumes=false;
GLuint shadowVolumeVP;
GLuint lightingVP;

//sphere
GLUquadricObj * sphere;

//Projection matrix
MATRIX4X4 projectionMatrix;

//Set up variables
bool DemoInit()
{
	if(!window.Init("Shadow Volumes", 640, 480, 32, 24, 8, WINDOWED_SCREEN))
		return 0;											//quit if not created

	//Set up extensions
	SetUpEXT_stencil_two_side();
	SetUpEXT_stencil_wrap();
	SetUpARB_vertex_program();

	if(EXT_stencil_two_side_supported && EXT_stencil_wrap_supported)
		twoSidedStencilSupported=true;

	if(ARB_vertex_program_supported)
		VPVolumesSupported=true;

	camera.Init(VECTOR3D(0.0f, 0.0f, 12.0f), 3.0f, 25.0f);

	//Set up models
	models[0].GenerateTorus(0.35f, 0.7f);
	models[1].GenerateTorus(0.3f, 1.0f);

	//Set model radii - could easily calculate these for more complex models
	modelRadii[0]=1.05f;
	modelRadii[1]=1.3f;

	//Set colors
	torus[0].color.Set(1.0f, 0.0f, 0.0f, 0.0f);		//small red torus
	torus[1].color.Set(1.0f, 0.0f, 0.0f, 0.0f);		//small red torus
	torus[2].color.Set(0.0f, 1.0f, 0.0f, 0.0f);		//larger green torus
	torus[3].color.Set(0.0f, 1.0f, 0.0f, 0.0f);		//larger green torus
	
	
	//Set up light
	light.position.Set(0.0f, 0.0f, 0.0f);
	light.color=white/1.5;

	sphere=gluNewQuadric();

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

	//set up projection matrix (infinite far plane)
	glMatrixMode(GL_PROJECTION);							//select projection matrix
	projectionMatrix.SetPerspective(45.0f, (GLfloat)window.width/(GLfloat)height, 1.0f, -1.0f);
	glLoadMatrixf(projectionMatrix);
	
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
	
	//Enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//Set up color material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//load vertex programs
	if(ARB_vertex_program_supported)
	{
		glGenProgramsARB(1, &shadowVolumeVP);
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, shadowVolumeVP);
		if(!LoadARB_program(GL_VERTEX_PROGRAM_ARB, "shadow volume VP.txt"))
			return false;
	
		glGenProgramsARB(1, &lightingVP);
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, lightingVP);
		if(!LoadARB_program(GL_VERTEX_PROGRAM_ARB, "lighting VP.txt"))
			return false;
	}

	return true;
}

//Perform per frame updates
void UpdateFrame()
{
	window.Update();
	camera.Update();

	//Toggle pause
	if(window.isKeyPressed('P'))
		timer.Pause();
	if(window.isKeyPressed('U'))
		timer.Unpause();

	//Switch to/from always using zFail
	if(window.isKeyPressed('F'))
		alwaysUseZFail=true;
	if(window.isKeyPressed('G'))
		alwaysUseZFail=false;

	//Switch to/from using 2 sided stencil
	if(window.isKeyPressed('1'))
		useTwoSidedStencil=false;
	if(window.isKeyPressed('2') && twoSidedStencilSupported)
		useTwoSidedStencil=true;

	//Switch to/from using vertex program volume generation
	if(window.isKeyPressed('V') && VPVolumesSupported)
		useVPVolumes=true;
	if(window.isKeyPressed('S'))
		useVPVolumes=false;

	//Update light position
	if(window.isKeyPressed(VK_UP) && light.position.y<4.5f)
		light.position.y+=0.05f;
	if(window.isKeyPressed(VK_DOWN) && light.position.y>-4.5f)
		light.position.y-=0.05f;

	//Update model positions and angles
	for(int i=0; i<numTori; ++i)
	{
		if(i==0 || i==1)
		{
			torus[i].position.Set(	2.0f*(float)sin(timer.GetTime()/1000 + M_PI*i),
									0.0f,
									2.0f*(float)cos(timer.GetTime()/1000 + M_PI*i));
			torus[i].angle=(float)timer.GetTime()/15;
		}
		else
		{
			torus[i].position.Set(	4.0f*(float)sin(-timer.GetTime()/800 + M_PI*i),
										 (float)sin(-timer.GetTime()/300 + M_PI*i),
									4.0f*(float)cos(-timer.GetTime()/800 + M_PI*i));
			//keep the green tori at the same angle to the light
			torus[i].angle=(float)timer.GetTime()*180/float(800*M_PI);
		}
	
		//Find the object space light positions
		MATRIX4X4 inverseModelMatrix;

		glPushMatrix();
		glLoadIdentity();
		glRotatef(-torus[i].angle, 0.0f, 0.0f, 1.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(-torus[i].position.x, -torus[i].position.y, -torus[i].position.z);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix);
		glPopMatrix();

		//Set the object space light positions
		torus[i].objectSpaceLightPosition=inverseModelMatrix*light.position;
	}
}

//draw a frame
void RenderFrame()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix

	//Set view
	camera.SetupViewMatrix();
	glMultMatrixf(camera.viewMatrix);

	//Calculate the viewProjection matrix (world -> clip coords)
	MATRIX4X4 viewProjectionMatrix=projectionMatrix*camera.viewMatrix;
	MATRIX4X4 inverseViewProjectionMatrix=viewProjectionMatrix.GetInverse();

	//Calculate the points in world space which become the corners of the near plane
	VECTOR3D corners[4];
	corners[0]=inverseViewProjectionMatrix*VECTOR3D( 1.0f, 1.0f,-1.0f);
	corners[1]=inverseViewProjectionMatrix*VECTOR3D(-1.0f, 1.0f,-1.0f);
	corners[2]=inverseViewProjectionMatrix*VECTOR3D(-1.0f,-1.0f,-1.0f);
	corners[3]=inverseViewProjectionMatrix*VECTOR3D( 1.0f,-1.0f,-1.0f);

	//Calculate the planes to check the model position against
	//				 /light
	//	------------o-------------  lightPlanes[5]
	//			   / \ 
	// lPlanes[0]-/ * \ - lightPlanes[1]
	//           /     \
	//  ---------eeeeeee----------  lightPlanes[4]
	//               \eye near plane
	//Plane normals face "inwards" (towards *). If the occluder is completely behind any plane,
	//there is no way its shadow can intersect the near plane, so no need for zfail technique
	//and hence no need for caps on volume
	PLANE lightPlanes[6];
	lightPlanes[0].SetFromPoints(light.position, corners[0], corners[1]);
	lightPlanes[1].SetFromPoints(light.position, corners[1], corners[2]);
	lightPlanes[2].SetFromPoints(light.position, corners[2], corners[3]);
	lightPlanes[3].SetFromPoints(light.position, corners[3], corners[0]);
	lightPlanes[4].SetFromPoints(corners[0], corners[2], corners[1]);	//near clip plane
	lightPlanes[5].SetFromPoints(corners[0], corners[1], corners[2]);	//"far clip plane"
	lightPlanes[5].CalculateIntercept(light.position);



	//Draw ambient
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(light.position)); 
	glLightfv(GL_LIGHT1, GL_AMBIENT, light.color/5);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light.color/5);
	glLightfv(GL_LIGHT1, GL_SPECULAR, black);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	//Enable vertex program if using VP for volumes
	if(useVPVolumes)
	{
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, lightingVP);
		glEnable(GL_VERTEX_PROGRAM_ARB);
	}

	//Draw box
	glColor3f(0.2f, 0.2f, 1.0f);
	DrawBox();

	//Draw models
	for(int i=0; i<numTori; ++i)
	{
		glColor4fv(torus[i].color);
		glPushMatrix();
		glTranslatef(torus[i].position.x, torus[i].position.y, torus[i].position.z);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(torus[i].angle, 0.0f, 0.0f, 1.0f);
		if(i==0 || i==1)
		{	
			glVertexPointer(3, GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[0].vertices[0].position);
			glNormalPointer(GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[0].vertices[0].normal);
			glDrawElements(GL_TRIANGLES, models[0].numIndices, GL_UNSIGNED_INT, models[0].indices);
		
		}
		else
		{
			glVertexPointer(3, GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[1].vertices[0].position);
			glNormalPointer(GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[1].vertices[0].normal);
			glDrawElements(GL_TRIANGLES, models[1].numIndices, GL_UNSIGNED_INT, models[1].indices);
		}
		glPopMatrix();
	}

	glPopAttrib();




	//Draw shadow volumes
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColorMask(0, 0, 0, 0);
	glShadeModel(GL_FLAT);
	glDepthMask(0);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);

	for(int i=0; i<numTori; ++i)
	{
		//See which model is used for this torus
		int currentModel;
		if(i==0 || i==1)
			currentModel=0;
		else
			currentModel=1;

		//Calculate silhouette edges for this torus if not using vertex program volumes
		if(!useVPVolumes)
			models[currentModel].CalculateSilhouetteEdges(torus[i].objectSpaceLightPosition);

		//If using vertex program volumes, bind the program and
		//Set up vertex pointers to draw models
		if(useVPVolumes)
		{
			glBindProgramARB(GL_VERTEX_PROGRAM_ARB, shadowVolumeVP);

			//Pass light position as env parameter 0
			glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 0, VECTOR4D(torus[i].objectSpaceLightPosition));

			glEnable(GL_VERTEX_PROGRAM_ARB);

			glVertexPointer(3, GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[currentModel].vertices[0].position);
			glNormalPointer(GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[currentModel].vertices[0].normal);
		}
			
		glPushMatrix();
		glTranslatef(torus[i].position.x, torus[i].position.y, torus[i].position.z);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(torus[i].angle, 0.0f, 0.0f, 1.0f);


		
		//If not always using zFail, see if it is necessary for this particular torus
		bool needZFail=true;

		if(!alwaysUseZFail)
		{
			//If the bounding sphere of the model is completely behind any of the lightPlanes,
			//no need for z fail.
			for(int currentPlane=0; currentPlane<6; ++currentPlane)
			{
				if(	lightPlanes[currentPlane].normal.x*torus[i].position.x+
					lightPlanes[currentPlane].normal.y*torus[i].position.y+
					lightPlanes[currentPlane].normal.z*torus[i].position.z+
					lightPlanes[currentPlane].intercept<-modelRadii[currentModel])
				{
					needZFail=false;
					break;
				}
			}
		}

		

		//Draw shadow volumes WITHOUT two sided stencil
		if(!useTwoSidedStencil)
		{
			//Using zfail
			if(alwaysUseZFail || needZFail)
			{
				//Increment stencil buffer for back face depth fail
				glStencilFunc(GL_ALWAYS, 0, ~0);
				glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
				glCullFace(GL_FRONT);
				//Draw the shadow volume if not using vertex program
				if(!useVPVolumes)
					models[currentModel].DrawInfiniteShadowVolume(	torus[i].objectSpaceLightPosition,
																	true);
				//Draw the model if using vertex program
				if(useVPVolumes)
					glDrawElements(GL_TRIANGLES, models[currentModel].numIndices, GL_UNSIGNED_INT, models[currentModel].indices);

			
				//Decrement stencil buffer for front face depth fail
				glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
				glCullFace(GL_BACK);
				//Draw the shadow volume if not using vertex program
				if(!useVPVolumes)
					models[currentModel].DrawInfiniteShadowVolume(	torus[i].objectSpaceLightPosition,
																	true);
				//Draw the model if using vertex program
				if(useVPVolumes)
					glDrawElements(GL_TRIANGLES, models[currentModel].numIndices, GL_UNSIGNED_INT, models[currentModel].indices);

			}
			else	//using zpass
			{
				//Increment stencil buffer for front face depth pass
				glStencilFunc(GL_ALWAYS, 0, ~0);
				glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
				glCullFace(GL_BACK);
				//Draw the shadow volume if not using vertex program
				if(!useVPVolumes)
					models[currentModel].DrawInfiniteShadowVolume(	torus[i].objectSpaceLightPosition,
																	false);
				//Draw the model if using vertex program
				if(useVPVolumes)
					glDrawElements(GL_TRIANGLES, models[currentModel].numIndices, GL_UNSIGNED_INT, models[currentModel].indices);


				//Decrement stencil buffer for back face depth pass
				glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
				glCullFace(GL_FRONT);
				//Draw the shadow volume if not using vertex program
				if(!useVPVolumes)
					models[currentModel].DrawInfiniteShadowVolume(	torus[i].objectSpaceLightPosition,
																	false);
				//Draw the model if using vertex program
				if(useVPVolumes)
					glDrawElements(GL_TRIANGLES, models[currentModel].numIndices, GL_UNSIGNED_INT, models[currentModel].indices);
			}
		}

		//Draw shadow volumes WITH two sided stencil
		if(useTwoSidedStencil)
		{
			//Using zfail
			if(alwaysUseZFail || needZFail)
			{
				glDisable(GL_CULL_FACE);
				
				//Increment(with wrapping) for back face depth fail
				glActiveStencilFaceEXT(GL_BACK);
				glStencilFunc(GL_ALWAYS, 0, ~0);
				glStencilOp(GL_KEEP, GL_INCR_WRAP_EXT, GL_KEEP);

				//Decrement(with wrapping) for front face depth fail
				glActiveStencilFaceEXT(GL_FRONT);
				glStencilFunc(GL_ALWAYS, 0, ~0);
				glStencilOp(GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP);

				//Enable 2 sided stencil
				glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

				//Draw the shadow volume if not using vertex program
				if(!useVPVolumes)
					models[currentModel].DrawInfiniteShadowVolume(	torus[i].objectSpaceLightPosition,
																	true);
				//Draw the model if using vertex program
				if(useVPVolumes)
					glDrawElements(GL_TRIANGLES, models[currentModel].numIndices, GL_UNSIGNED_INT, models[currentModel].indices);

			}
			else	//using zpass
			{
				glDisable(GL_CULL_FACE);

				//Increment(with wrapping) for front face depth pass
				glActiveStencilFaceEXT(GL_FRONT);
				glStencilFunc(GL_ALWAYS, 0, ~0);
				glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);

				//Decrement(with wrapping) for back face depth pass
				glActiveStencilFaceEXT(GL_BACK);
				glStencilFunc(GL_ALWAYS, 0, ~0);
				glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);

				//Enable 2 sided stencil
				glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

				//Draw the shadow volume if not using vertex program
				if(!useVPVolumes)
					models[currentModel].DrawInfiniteShadowVolume(	torus[i].objectSpaceLightPosition,
																	false);
				//Draw the model if using vertex program
				if(useVPVolumes)
					glDrawElements(GL_TRIANGLES, models[currentModel].numIndices, GL_UNSIGNED_INT, models[currentModel].indices);
			}
		}

		glPopMatrix();
	}

	glPopAttrib();

	


	//Now draw lit where stencil==0
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(light.position)); 
	glLightfv(GL_LIGHT1, GL_AMBIENT, light.color/5);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light.color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glStencilFunc(GL_EQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_STENCIL_TEST);

	//Enable vertex program if using VP for volumes
	if(useVPVolumes)
	{
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, lightingVP);
		glEnable(GL_VERTEX_PROGRAM_ARB);
	}

	//Draw box
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glColor3f(0.2f, 0.2f, 1.0f);
	DrawBox();

	//Draw models
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
	for(int i=0; i<numTori; ++i)
	{
		glColor4fv(torus[i].color);
		glPushMatrix();
		glTranslatef(torus[i].position.x, torus[i].position.y, torus[i].position.z);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(torus[i].angle, 0.0f, 0.0f, 1.0f);
		if(i==0 || i==1)
		{	
			glVertexPointer(3, GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[0].vertices[0].position);
			glNormalPointer(GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[0].vertices[0].normal);
			glDrawElements(GL_TRIANGLES, models[0].numIndices, GL_UNSIGNED_INT, models[0].indices);
		
		}
		else
		{
			glVertexPointer(3, GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[1].vertices[0].position);
			glNormalPointer(GL_FLOAT, sizeof(SHADOW_MODEL_VERTEX), &models[1].vertices[0].normal);
			glDrawElements(GL_TRIANGLES, models[1].numIndices, GL_UNSIGNED_INT, models[1].indices);
		}
		glPopMatrix();
	}

	glPopAttrib();



	//Draw sphere at light's position
	glPushMatrix();
	glColor4fv(light.color);
	glTranslatef(light.position.x, light.position.y, light.position.z);
	gluSphere(sphere, 0.1, 24, 24);
	glPopMatrix();


	fpsCounter.Update();											//update frames per second counter
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	window.StartTextMode();
	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	window.Print(0, 48, "%s", alwaysUseZFail ?	"\"zFail\" algorithm with capped, infinite shadow volumes" :
												"\"zPass\" algorithm with non-capped volumes, where possible");
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	window.Print(0, 68, "%s", useVPVolumes ?	"Extruding shadow volumes in a vertex program" :
												"Standard CPU shadow volume calculations");
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	if(useTwoSidedStencil)
		window.Print(0, 88, "Using EXT_stencil_two_side");
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