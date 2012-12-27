//////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
// CS 458
// Assignment #5
// Created 10/23/2000
// Last updated 10/26/2000
//
//	This program creates a scene of a fractal mountain, a
//	lighthouse that was created by rotating a line around 
//	the y-axis, and an ocean with waves generated using a
//	sin curve.
//
//	The program uses OpenGL for the rendering and Glut for
//	all the Windowing system initialization.
//
//////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "FractalMtn.h"
#include "RotationalObj.h"
#include "Ocean.h"
#include "View.h"

// Forward declaration of functions
void Initialize();
void RenderScene();
void OnResize(int w, int h);
void OnMouseButton(int button, int state, int x, int y);
void OnIdle();

enum CAMERA_MODE
{
	CAMERA_IDLE,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	ROTATE_UP,
	ROTATE_DOWN
};

// Globals to store window state
int g_nWidth;
int g_nHeight;
CAMERA_MODE g_eMode;

// Objects in the scene
CFractalMtn g_mtn1;
CRotationalObj g_LightHouse;
COcean g_Ocean;

// Main entry point for the program
int main(int argc, char* argv[])
{
	// window initialization
	View * v = new View(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Open a window
	v->createWindow("CS458 - Assignment 5", 640, 480);

	// Initialize OpenGL and objects
	Initialize();

	// Setup callback functions
	v->onIdle(OnIdle);
	v->onMouse(OnMouseButton);
	v->onDraw(RenderScene);
	v->onResize(OnResize);

	// Start Glut window
	v->start();
	return 0;
}

// Load the ocean
void LoadOcean()
{
	Point3 pos = {0.0, 0.5, 5.0};

	g_Ocean.Create(55, 35, 256, 4);

#ifdef WIN32
	g_Ocean.LoadTexture("../textures/ocean.bmp");
#else
	g_Ocean.LoadTexture("../textures/ocean.ppm");
#endif
	g_Ocean.PreCache();
	g_Ocean.SetPosition(pos);
}

// Load the lighthouse
void LoadLighthouse()
{
	Point3List vertList;
	// Points calculated by hand to sweep out the lighthouse
	Point3 verts[14] = {
		{0.0, 10.0, 0.0}, {-2.0, 8.5, 0.0},{-1.3, 8.5, 0.0},
		{-1.4, 7.5, 0.0},{-1.3, 6.5, 0.0}, {-2.0, 6.5, 0.0},
		{-2.0, 6.75, 0.0}, {-2.0, 6.75, 0.0}, {-2.0, 6.4, 0.0},
		{-1.3, 6.4, 0.0}, {-1.45, 4.0, 0.0}, {-1.6, 2.0, 0.0},
		{-1.9, 0.0, 0.0}, {0.0, 0.0, 0.0}};
	
	// Position the lighthouse in the appropriate spot
	Point3 pos = {20.5, 3.2, -8.7};

	// Load vertices into list
	for (int c=0;c<14;c++)
		vertList.push_back(verts[c]);

	// Create the lighthouse, precache it, and set the position
	g_LightHouse.RotateLine(vertList, 16);
#ifdef WIN32
	g_LightHouse.LoadTexture("../textures/lighthouse.bmp");
#else
	g_LightHouse.LoadTexture("../textures/lighthouse.ppm");
#endif
	g_LightHouse.PreCache();
	g_LightHouse.SetPosition(pos);
}

// Load the landscape
void LoadMountain()
{
	// Initial landscape face and vertex data generated in 
	// 3dsMAX and exported with a MAXScript script.
	Point3 verts[125] = {
		{-30.0f, -5.79f, 19.38f},	{-24.0f, -5.79f, 19.38f},	{-18.0f, -5.79f, 19.38f},
		{-12.0f, -5.79f, 19.38f},	{-6.0f, -5.79f, 19.38f},	{0.0f, -5.79f, 19.38f},
		{5.99f, -5.79f, 19.38f},	{11.99f, -5.79f, 19.38f},	{17.99f, -5.79f, 19.38f},
		{23.99f, -5.79f, 19.38f},	{29.99f, -5.79f, 19.38f},	{-30.0f, -4.17f, 15.72f},
		{-24.0f, -4.17f, 15.72f},	{-18.0f, -4.17f, 15.72f},	{-12.0f, -4.17f, 15.72f},
		{-6.0f, -4.17f, 15.72f},	{0.0f, -4.17f, 15.72f},	{5.99f, -4.17f, 15.72f},
		{11.99f, -4.17f, 15.72f},	{17.99f, -4.17f, 15.72f},	{23.99f, -4.17f, 15.72f},
		{29.99f, -4.17f, 15.72f},	{-30.0f, -2.54f, 12.07f},	{-24.0f, -2.54f, 12.07f},
		{-18.0f, -2.54f, 12.07f},	{-12.0f, -2.54f, 12.07f},	{-6.0f, -2.54f, 12.07f},
		{0.0f, -2.54f, 12.07f},	{5.99f, -2.54f, 12.07f},	{11.99f, -2.54f, 12.07f},
		{17.99f, -2.54f, 12.07f},	{23.99f, -2.54f, 12.07f},	{29.99f, -2.54f, 12.07f},
		{-30.0f, -1.74f, 7.57f},	{-24.0f, -1.74f, 7.57f},	{-18.0f, -1.74f, 7.57f},
		{-12.0f, -1.74f, 7.57f},	{-6.0f, -1.74f, 7.57f},	{0.0f, -1.74f, 7.57f},
		{5.99f, -1.96f, 7.57f},	{11.99f, -1.74f, 7.57f},	{17.99f, -1.74f, 7.57f},
		{23.99f, -1.74f, 7.57f},	{29.99f, -1.74f, 7.57f},	{-30.0f, -0.86f, 3.57f},
		{-24.0f, -0.86f, 3.57f},	{-18.0f, -0.86f, 3.57f},	{-12.0f, -0.86f, 3.57f},
		{-6.0f, -0.86f, 3.57f},	{0.0f, -0.86f, 3.57f},	{5.99f, -0.86f, 3.57f},
		{11.99f, -0.86f, 3.57f},	{17.99f, -0.86f, 3.57f},	{23.99f, -0.86f, 3.57f},
		{29.99f, -0.86f, 3.57f},	{-30.0f, 0.0f, -0.42f},	{-24.0f, 0.0f, -0.42f},
		{-18.0f, 0.0f, -0.42f},	{-12.0f, 0.0f, -0.42f},	{-6.0f, 0.0f, -0.42f},
		{0.0f, 0.0f, -0.42f},	{5.99f, 0.0f, -0.42f},	{11.99f, 0.0f, -0.42f},
		{17.99f, 0.0f, -0.42f},	{23.99f, 0.0f, -0.42f},	{29.99f, 0.0f, -0.42f},
		{-30.0f, -0.03f, -4.42f},	{-24.0f, -0.03f, -4.42f},	{-18.0f, -0.03f, -4.42f},
		{-12.0f, -0.03f, -4.42f},	{-6.0f, -0.03f, -4.42f},	{0.0f, -0.03f, -4.42f},
		{5.99f, -0.03f, -4.42f},	{11.99f, -0.03f, -4.42f},	{17.99f, 3.16f, -4.42f},
		{23.99f, 3.16f, -4.42f},	{29.99f, -0.03f, -4.42f},	{-30.0f, -0.06f, -8.42f},
		{-24.2f, 11.32f, -8.51f},	{-18.2f, 11.32f, -8.51f},	{-12.0f, -0.06f, -8.42f},
		{-6.0f, -0.06f, -8.42f},	{0.0f, -0.06f, -8.42f},	{5.99f, -0.06f, -8.42f},
		{11.99f, -0.06f, -8.42f},	{17.99f, 3.13f, -8.42f},	{23.99f, 3.13f, -8.42f},
		{29.99f, -0.06f, -8.42f},	{-30.0f, -0.1f, -12.41f},	{-24.18f, 10.58f, -12.51f},
		{-18.18f, 10.58f, -12.51f},	{-12.2f, 11.29f, -12.51f},	{-6.0f, -0.1f, -12.41f},
		{0.0f, -0.1f, -12.41f},	{5.99f, -0.1f, -12.41f},	{11.99f, -0.1f, -12.41f},
		{17.99f, -0.1f, -12.41f},	{23.99f, -0.1f, -12.41f},	{29.99f, -0.1f, -12.41f},
		{-30.0f, -0.13f, -16.41f},	{-24.18f, 10.54f, -16.51f},	{-18.18f, 10.54f, -16.51f},
		{-12.2f, 11.25f, -16.51f},	{-6.0f, 5.19f, -16.41f},	{0.0f, 5.19f, -16.41f},
		{5.99f, 7.68f, -16.41f},	{11.99f, 7.68f, -16.41f},	{17.99f, 7.68f, -16.41f},
		{23.99f, 7.68f, -16.41f},	{29.99f, -0.13f, -16.41f},	{-30.0f, -0.17f, -20.41f},
		{-24.0f, -0.17f, -20.41f},	{-18.0f, -0.17f, -20.41f},	{-12.0f, -0.17f, -20.41f},
		{-6.0f, -0.17f, -20.41f},	{0.0f, -0.17f, -20.41f},	{5.99f, -0.17f, -20.41f},
		{11.99f, -0.17f, -20.41f},	{17.99f, -0.17f, -20.41f},	{23.99f, -0.17f, -20.41f},
		{29.99f, -0.17f, -20.41f},	{0.0f, -1.74f, 7.57f},	{0.0f, -2.42f, 7.57f},
		{5.99f, -1.74f, 7.57f},	{0.0f, -2.64f, 7.57f}
	};
	Face faces[200] = {
		{11, 0, 12},	{1, 12, 0},	{12, 1, 13},
		{2, 13, 1},	{13, 2, 14},	{3, 14, 2},
		{14, 3, 15},	{4, 15, 3},	{15, 4, 16},
		{5, 16, 4},	{16, 5, 17},	{6, 17, 5},
		{17, 6, 18},	{7, 18, 6},	{18, 7, 19},
		{8, 19, 7},	{19, 8, 20},	{9, 20, 8},
		{20, 9, 21},	{10, 21, 9},	{22, 11, 23},
		{12, 23, 11},	{23, 12, 24},	{13, 24, 12},
		{24, 13, 25},	{14, 25, 13},	{25, 14, 26},
		{15, 26, 14},	{26, 15, 27},	{16, 27, 15},
		{27, 16, 28},	{17, 28, 16},	{28, 17, 29},
		{18, 29, 17},	{29, 18, 30},	{19, 30, 18},
		{30, 19, 31},	{20, 31, 19},	{31, 20, 32},
		{21, 32, 20},	{33, 22, 34},	{23, 34, 22},
		{34, 23, 35},	{24, 35, 23},	{35, 24, 36},
		{25, 36, 24},	{36, 25, 37},	{26, 37, 25},
		{37, 26, 38},	{27, 38, 26},	{38, 27, 39},
		{28, 39, 27},	{39, 28, 40},	{29, 40, 28},
		{40, 29, 41},	{30, 41, 29},	{41, 30, 42},
		{31, 42, 30},	{42, 31, 43},	{32, 43, 31},
		{44, 33, 45},	{34, 45, 33},	{45, 34, 46},
		{35, 46, 34},	{46, 35, 47},	{36, 47, 35},
		{47, 36, 48},	{37, 48, 36},	{48, 37, 49},
		{38, 49, 37},	{49, 38, 50},	{39, 50, 38},
		{50, 39, 51},	{40, 51, 39},	{51, 40, 52},
		{41, 52, 40},	{52, 41, 53},	{42, 53, 41},
		{53, 42, 54},	{43, 54, 42},	{55, 44, 56},
		{45, 56, 44},	{56, 45, 57},	{46, 57, 45},
		{57, 46, 58},	{47, 58, 46},	{58, 47, 59},
		{48, 59, 47},	{59, 48, 60},	{49, 60, 48},
		{60, 49, 61},	{50, 61, 49},	{61, 50, 62},
		{51, 62, 50},	{62, 51, 63},	{52, 63, 51},
		{63, 52, 64},	{53, 64, 52},	{64, 53, 65},
		{54, 65, 53},	{66, 55, 67},	{56, 67, 55},
		{67, 56, 68},	{57, 68, 56},	{68, 57, 69},
		{58, 69, 57},	{69, 58, 70},	{59, 70, 58},
		{70, 59, 71},	{60, 71, 59},	{71, 60, 72},
		{61, 72, 60},	{72, 61, 73},	{62, 73, 61},
		{73, 62, 74},	{63, 74, 62},	{74, 63, 75},
		{64, 75, 63},	{75, 64, 76},	{65, 76, 64},
		{77, 66, 78},	{67, 78, 66},	{78, 67, 79},
		{68, 79, 67},	{79, 68, 80},	{69, 80, 68},
		{80, 69, 81},	{70, 81, 69},	{81, 70, 82},
		{71, 82, 70},	{82, 71, 83},	{72, 83, 71},
		{83, 72, 84},	{73, 84, 72},	{84, 73, 85},
		{74, 85, 73},	{85, 74, 86},	{75, 86, 74},
		{86, 75, 87},	{76, 87, 75},	{88, 77, 89},
		{78, 89, 77},	{89, 78, 90},	{79, 90, 78},
		{90, 79, 91},	{80, 91, 79},	{91, 80, 92},
		{81, 92, 80},	{92, 81, 93},	{82, 93, 81},
		{93, 82, 94},	{83, 94, 82},	{94, 83, 95},
		{84, 95, 83},	{95, 84, 96},	{85, 96, 84},
		{96, 85, 97},	{86, 97, 85},	{97, 86, 98},
		{87, 98, 86},	{99, 88, 100},	{89, 100, 88},
		{100, 89, 101},	{90, 101, 89},	{101, 90, 102},
		{91, 102, 90},	{102, 91, 103},	{92, 103, 91},
		{103, 92, 104},	{93, 104, 92},	{104, 93, 105},
		{94, 105, 93},	{105, 94, 106},	{95, 106, 94},
		{106, 95, 107},	{96, 107, 95},	{107, 96, 108},
		{97, 108, 96},	{108, 97, 109},	{98, 109, 97},
		{110, 99, 111},	{100, 111, 99},	{111, 100, 112},
		{101, 112, 100},	{112, 101, 113},	{102, 113, 101},
		{113, 102, 114},	{103, 114, 102},	{114, 103, 115},
		{104, 115, 103},	{115, 104, 116},	{105, 116, 104},
		{116, 105, 117},	{106, 117, 105},	{117, 106, 118},
		{107, 118, 106},	{118, 107, 119},	{108, 119, 107},
		{119, 108, 120},	{109, 120, 108}
	};

	Point3List vertList;
	FaceList faceList;
	int c;

	// Position the landscape a bit to line everything up
	Point3 pos = {0.0, -0.75, -2.8};

	// Load the vertices into a list
	for (c=0;c<125;c++)
		vertList.push_back(verts[c]);

	// Load the faces into a list
	for (c=0;c<200;c++)
		faceList.push_back(faces[c]);

	// Load a texture
#ifdef WIN32
	g_mtn1.LoadTexture("../textures/landscape.bmp");
#else
	g_mtn1.LoadTexture("../textures/landscape.ppm");
#endif

	// Create the fractal using 5 recursion levels
	g_mtn1.CreateFractal(vertList, faceList, 3);

	// Pre-render and set the position
	g_mtn1.PreCache();
	g_mtn1.SetPosition(pos);
}

// Initialize OpenGL settings
void Initialize()
{
	// Light direction
	float fLightPos[4] = {0.0, 0.9, 0.75, 0.0};

	// Misc OGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Set a directional light
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

	// set clear color to a blue
	glClearColor(0.0, 0.0, 0.6, 1.0);

	// Set alpha blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load the models
	LoadMountain();
	LoadLighthouse();
	LoadOcean();
}

// Renders the scene
void RenderScene()
{
	// Material colors for objects
	float fOceanAmb[4] = {1.0, 1.0, 1.0, 0.75};
	float fLightHouse[4] = {1.0, 1.0, 1.0, 1.0};
	float fWhiteMat[4] = {1.0, 1.0, 1.0, 1.0};

	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set material for and render landscape
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, fWhiteMat);
	g_mtn1.Render();

	// Set material for and render lighthouse
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, fLightHouse);
	g_LightHouse.Render();

	// Set material for and render ocean
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, fOceanAmb);
	g_Ocean.Render();

	glFlush();

	// Flip double buffers
	glutSwapBuffers();
}

// Resize function.  Sets the projection matrix and viewport sizes
// Also initially sets a transformation to emulate positioning the
// camera for the combined model-view matrix
void OnResize(int w, int h)
{
	float fLeft, fRight, fBottom, fTop, fNear, fFar;

	// Calculate projection variables
	fLeft = -12.0;
	fRight = -fLeft;
	fBottom = fLeft * static_cast<float>(h) / static_cast<float>(w);
	fTop = -fBottom;
	fNear = 15.0;
	fFar = 100.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(fLeft, fRight, fBottom, fTop, fNear, fFar);
	glViewport(0.0, 0.0, w, h);

	// Position "camera"
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, -9.5, -37.0);

	g_nHeight = h;
	g_nWidth = w;

	// Redisplay the scene
	glutPostRedisplay();
}

// mouse button callback for handling rotations
void OnMouseButton(int button, int state, int x, int y)
{
	float yAlpha = x - g_nWidth/2;
	float xAlpha = -y + g_nHeight/2;

	if (state == GLUT_DOWN)
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			if (x > g_nWidth/2)
				g_eMode = ROTATE_LEFT;
			else
				g_eMode = ROTATE_RIGHT;
			break;
		case GLUT_RIGHT_BUTTON:
			if (y > g_nWidth/2)
				g_eMode = ROTATE_UP;
			else
				g_eMode = ROTATE_DOWN;
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		}
	}
	else
	{
		g_eMode = CAMERA_IDLE;
	}
}

// If mouse button is pressed rotate, otherwise do nothing
void OnIdle()
{
	switch(g_eMode)
	{
	case CAMERA_IDLE:
		// do nothing
		break;
	case ROTATE_LEFT:
		glRotatef(2.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case ROTATE_RIGHT:
		glRotatef(-2.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case ROTATE_UP:
		glRotatef(2.0, 1.0, 0.0, 0.0);
		glutPostRedisplay();
		break;
	case ROTATE_DOWN:
		glRotatef(-2.0, 1.0, 0.0, 0.0);
		glutPostRedisplay();
		break;
	}
}
