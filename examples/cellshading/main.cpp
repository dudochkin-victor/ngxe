//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Cel Shading, with ARB_vertex_program
//	Downloaded from: www.paulsprojects.net
//	Created:	26th August 2002
//	Updated:	2nd October 2002	-	Use ARB_vertex_program
//				13th February 2003	-	Fixed bug when running on ATI Radeon 9000
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "extensions/EXT_compiled_vertex_array_extension.h"
#include "extensions/EXT_texture_edge_clamp_extension.h"
#include "extensions/ARB_vertex_program_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Util.h"
#include "View.h"
#include "Maths/Maths.h"
#include "main.h"
#include "TORUS.h"
#include "ARB_program.h"

//errorLog MUST be kept - it is used by other files
FPS_COUNTER fpsCounter;
TIMER timer;

//Colors
COLOR backgroundColor(0.2f, 0.627f, 0.055f, 1.0f);
COLOR objectColor(0.75f, 0.95f, 1.0f, 1.0f);

float * lightCoefficients;	//store brightness (n.l) at each vertex

GLuint celBrightness;

TORUS torus;

//light position(world space)
VECTOR3D lightPosition(5.0f, 5.0f, 5.0f);

//outline width
float lineWidth;
float lineWidths[2];	//max, min

//for drawing light
GLUquadricObj * sphere;

bool vertexProgramSupported=false;
bool useVertexProgram=false;

GLuint vertexProgram;

//Set up variables
bool DemoInit()
{
	SetUpEXT_compiled_vertex_array();
	SetUpEXT_texture_edge_clamp();
	SetUpARB_vertex_program();
	
	if(!GL_EXT_texture_edge_clamp || !GL_EXT_compiled_vertex_array)
		return false;

	if(GL_ARB_vertex_program)
		vertexProgramSupported=true;

	//Load Textures
	GLubyte celBrightnessData[16]={	127, 127, 127, 191, 191, 191, 191, 191,
									255, 255, 255, 255, 255, 255, 255, 255};
	glGenTextures(1, &celBrightness);
	glBindTexture(GL_TEXTURE_1D, celBrightness);
	glTexImage1D(	GL_TEXTURE_1D, 0, GL_RGBA, 16,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, celBrightnessData);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	
	//get max line width
	glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidths);
	//set line width
	//line width is maximum of 5 and max line width
	lineWidth=lineWidths[1]>5.0f ? 5.0f : lineWidths[1]; 

	//set up sphere
	sphere=gluNewQuadric();

	//reset timer for start
	timer.Reset();
	
	return true;
}

//Set up openGL
bool GLInit()
{
	//set viewport
	int height = 480;
	int width = 640;
	
	glViewport(0, 0, width, height);					//reset viewport

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);							//select projection matrix
	glLoadIdentity();										//reset
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f);
	
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

	//Create space for light brightness
	lightCoefficients=new float[torus.numVertices];
	if(!lightCoefficients)
	{
		Util::log("Unable to allocate space for light coefficients");
		return false;
	}

	//Set vertex pointers
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].position);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(1, GL_FLOAT, 0, lightCoefficients);

	glNormalPointer(GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].normal);

	glLockArraysEXT(0, torus.numVertices);

	//Set line width
	glLineWidth(lineWidth);

	//Load vertex program
	if(vertexProgramSupported)
	{
		glGenProgramsARB(1, &vertexProgram);
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertexProgram);
		if(!LoadARB_program(GL_VERTEX_PROGRAM_ARB, "vertex program.txt"))
			return false;
	}
	
	return true;
}

//Perform per frame updates
void UpdateFrame(unsigned char key, int x, int y)
{
	//change line width
//	if(window.isKeyPressed(VK_UP) && lineWidth<lineWidths[1])
//	{
//		lineWidth++;
//		glLineWidth(lineWidth);
//	}
//
//	if(window.isKeyPressed(VK_DOWN) && lineWidth>1.0f)
//	{
//		lineWidth--;
//		glLineWidth(lineWidth);
//	}
//
//	//toggle vertex program
//	if(window.isKeyPressed('1'))
//	{
//		glUnlockArraysEXT();
//
//		useVertexProgram=false;
//		//for no vertex program, need to know light coefficients
//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//		//dont need normals
//		glDisableClientState(GL_NORMAL_ARRAY);
//
//		glLockArraysEXT(0, torus.numVertices);
//	}
//
//	if(window.isKeyPressed('2') && vertexProgramSupported==true)
//	{
//		glUnlockArraysEXT();
//
//		useVertexProgram=true;
//		//for vertex program, no need to know light coefficients
//		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//		//need normals
//		glEnableClientState(GL_NORMAL_ARRAY);
//
//		glLockArraysEXT(0, torus.numVertices);
//	}

}

//draw a frame
void RenderFrame()
{
	static double angleYaw=-22.0, anglePitch=28.0;
	static MATRIX4X4 inverseModelMatrix;

	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										//reset modelview matrix

	glTranslatef(0.0f, 0.0f, -20.0f);

	//rotate the model with the mouse
//	if(window.isLeftButtonPressed())
//	{
//		angleYaw+=(double)window.GetMouseXMovement()/3;
//		anglePitch+=(double)window.GetMouseYMovement()/3;
//	}

	//calculate the inverse model matrix by doing model transformations in reverse
	glPushMatrix();
	glLoadIdentity();
	glRotated(angleYaw, 0.0f, 0.0f, 1.0f);
	glRotated(-anglePitch, 1.0f, 0.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix);
	glPopMatrix();
	
	//use the inverse model matrix to convert light position into model space
	VECTOR3D modelSpaceLightPosition=inverseModelMatrix*lightPosition;

	//if no vertex program update light coefficients
	if(!useVertexProgram)
	{
		glUnlockArraysEXT();

		for(int i=0; i<torus.numVertices; i++)
		{
			VECTOR3D lightDirection=(modelSpaceLightPosition-torus.vertices[i].position).
									GetNormalized();

			lightCoefficients[i]=torus.vertices[i].normal.DotProduct(lightDirection);
		}

		glLockArraysEXT(0, torus.numVertices);
	}
		
	glPushMatrix();

	//apply model transformations
	glRotated(anglePitch, 1.0f, 0.0f, 0.0f);
	glRotated(-angleYaw, 0.0f, 0.0f, 1.0f);
	


	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor4fv(objectColor);
	glBindTexture(GL_TEXTURE_1D, celBrightness);
	glEnable(GL_TEXTURE_1D);

	if(useVertexProgram)
	{
		glEnable(GL_VERTEX_PROGRAM_ARB);
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertexProgram);

		//Send object space light vector as program env parameter 0
		glProgramEnvParameter4fvARB(GL_VERTEX_PROGRAM_ARB, 0, VECTOR4D(modelSpaceLightPosition));
	}
	
	//draw torus
	glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);

	glPopAttrib();




	//draw outline
	glCullFace(GL_FRONT);
	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);
	glPolygonMode(GL_BACK, GL_FILL);
	glCullFace(GL_BACK);

	glPopMatrix();

	//draw light
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glPushMatrix();
	glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
	gluSphere(sphere, 0.25f, 12, 24);
	glPopMatrix();

	fpsCounter.Update();											//update frames per second counter
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
//	window.StartTextMode();
//	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
//	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
//	window.Print(0, 48, "%s", useVertexProgram ?	"Using ARB_vertex_program" :
//													"Manual per vertex lighting");
//	window.EndTextMode();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

//	if(window.isKeyPressed(VK_F1))
//	{
//		window.SaveScreenshot();
//		window.SetKeyReleased(VK_F1);
//	}

	glutSwapBuffers();									//swap buffers

	//check for any opengl errors
//	window.CheckGLError();
//
//	//quit if necessary
//	if(window.isKeyPressed(VK_ESCAPE))
//		PostQuitMessage(0);
}

void DemoShutdown()
{
//	window.Shutdown();										//Shutdown window
}

int main(int argc, char **argv) {
	Util::log("%s" , "TEST");
	View * v = new View(&argc, argv);
	// Open a window
	v->createWindow("cellshading", 640, 480);

	//init variables etc, then GL
	if(!DemoInit())
	{
		Util::log("Demo Initiation failed");
		return 0;
	}
	else
		Util::log("Demo Initiation Successful");

	if(!GLInit())
	{
		Util::log("OpenGL Initiation failed");
		return 0;
	}
	else
		Util::log("OpenGL Initiation Successful");

	v->onDraw(&RenderFrame);
	v->onKeyboard(&UpdateFrame);
	v->start();

	DemoShutdown();
	
	Util::log("Exiting...");
	return 0;								//Exit The Program
}
