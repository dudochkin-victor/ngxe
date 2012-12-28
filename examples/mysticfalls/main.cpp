//////////////////////////////////////////////////////////////
// Created: 11-30-2000
// Last Modified: 12-10-2000
//
// This program builds a waterfall scene using several different
// techniques.  In order to get the elevation, a grayscale
// image is used as a heightmap.  The image ranges from the low
// black pixels, to the high white pixels.  To get the coloration
// of the land, a texture is used on the landscape's faces. The
// water is made up of several textured planes.  The location 
// and image name of these planes is read from a text file. To
// simulate the water spray from the waterfall, a simple particle
// effect is used.  Before rendering the scene, a textured quad
// is drawn to simultaneously clear the scene and apply a 
// background sky image.
//////////////////////////////////////////////////////////////
#include <string.h>
#include <GL/glut.h>
#ifdef WIN32
#include <GL/glaux.h>
#else
struct AUX_RGBImageRec
{
	int sizeX;
	int sizeY;
	GLubyte* data;
};

AUX_RGBImageRec* auxDIBImageLoad(const char* filename);
#endif
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

const float SCALE_X = 0.75;			// Amount to scale image in the X-plane
const float SCALE_Y = 0.75;			// Amount to scale image's Y-plane (scene's Z plane)
const float SCALE_Z = 0.25;			// Amount to scale the height.

const double PI = 3.1415926535898;		// Constant for PI
const double SIN1 = 0.017452406437284;	// Constant for sin(1)
const double COS1 = 0.99984769515639;	// Constant for cos(1)

// A texture list will be used to store all texture numbers
// in order to easily clean up texture memory on closing.
typedef std::vector<GLuint> TexList;

// Glut callback functions
void OnDisplay();
void OnReshape(int width, int height);
void OnKeyboard(unsigned char key, int x, int y);
void OnIdle();

// Initialization function
void Init();

// Display list IDs
GLuint g_nLand;
GLuint g_nWater;
GLuint g_nSpray;

// Background texture ID
GLuint g_nBackgroundTexture;

// Camera parameters
GLfloat g_pCameraPos[3] = {-80.0, 60.0, 0.0};
GLfloat g_pCameraOrient[3] = {0.0, 60.0, 10.0};
GLfloat g_fCameraFOV = 60;

// Width & height of window
GLuint g_uWidth;
GLuint g_uHeight;

// Scene rotation
GLfloat g_fLandRot = 0.0;

// List of textures used
TexList g_vecTextures;

// Wireframe mode
bool g_bWireframe = false;

using namespace std;

// Main entry point for the program
int main(int argc, char** argv)
{
	// Initialize Glut window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Dan Jenkins - CS458 - Assignment #8 - Mystic Falls");

	// Initialize OpenGL settings
	Init();

	// Register Glut callback functions
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyboard);
	glutIdleFunc(OnIdle);

	// Open window and begin rendering
	glutMainLoop();

	// Before exiting program delete textures and display lists
	glDeleteTextures(g_vecTextures.size(), &g_vecTextures[0]);
	glDeleteLists(g_nLand, 1);
	glDeleteLists(g_nWater, 1);
	glDeleteLists(g_nSpray, 1);

	return 0;
}

// GetFaceNormal - takes 3 vertex locations as  input and
// returns the normal for that face in norm
void GetFaceNormal(float v1[], float v2[], float v3[], float norm[])
{
	float temp1[3], temp2[3];
	float len;

	// temp1 = v1 - v2
	// temp2 = v2 - v3
	for (int c=0;c<3;c++)
	{
		temp1[c] = v1[c] - v2[c];
		temp2[c] = v2[c] - v3[c];
	}

	// norm = temp1 X temp2
	norm[0] = temp1[1]*temp2[2] - temp1[2]*temp2[1];
	norm[1] = temp1[2]*temp2[0] - temp1[0]*temp2[2];
	norm[2] = temp1[0]*temp2[1] - temp1[1]*temp2[2];
	
	// Normalize length of normal to a unit length of 1
	len = sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);

	norm[0] /= len;
	norm[1] /= len;
	norm[2] /= len;
}

// GetNormal - Returns the averaged normal for a given point in the
// heightmap image.  This function gets the normal for each face
// associated with that point and then averages them.
void GetNormal(AUX_RGBImageRec* pMap, int x, int y, float norm[])
{
	float v[3][3], temp[3];
	int nIndex[3];

	int c;

	// Initialize normal to 0,0,0
	norm[0] = norm[1] = norm[2] = 0.0;

	// Get face normal for the lower right face
	if (x < pMap->sizeX - 1 && y < pMap->sizeY - 1)
	{
		// Calculate image indices for face
		nIndex[0] = (y * pMap->sizeX + x) * 3;
		nIndex[1] = ((y+1)*pMap->sizeX + x) * 3;
		nIndex[2] = (y * pMap->sizeX + x + 1) * 3;

		// Calculate vertices for face
		for (c=0;c<3;c++)
		{
			v[c][0] = (x - pMap->sizeX/2) * SCALE_X;
			v[c][2] = (y - pMap->sizeY/2) * SCALE_Y;
			v[c][1] = pMap->data[nIndex[c]] * SCALE_Z;
		}
		
		// Calculate differing vertices for other 2 verts
		v[1][2] = ((y+1) - pMap->sizeY/2) * SCALE_Y;
		v[2][0] = ((x+1) - pMap->sizeX/2) * SCALE_X;

		// Get the normal for this face
		GetFaceNormal(v[0], v[1], v[2], temp);

		// Add normal of this face to the final normal
		norm[0] += temp[0];
		norm[1] += temp[1];
		norm[2] += temp[2];
	}

	// Get face normal for the lower left faces
	if (x > 0 && y < pMap->sizeY - 1)
	{
		// Calculate image indices for face
		nIndex[0] = (y * pMap->sizeX + x) * 3;
		nIndex[1] = ((y+1)*pMap->sizeX + x - 1) * 3;
		nIndex[2] = ((y+1) * pMap->sizeX + x) * 3;

		// Calculate vertices for face
		for (c=0;c<3;c++)
		{
			v[c][0] = (x - pMap->sizeX/2) * SCALE_X;
			v[c][2] = (y - pMap->sizeY/2) * SCALE_Y;
			v[c][1] = pMap->data[nIndex[c]] * SCALE_Z;
		}

		// Calculate differing vertices for other 2 verts
		v[1][2] = ((y+1) - pMap->sizeY/2) * SCALE_Y;
		v[1][0] = ((x-1) - pMap->sizeX/2) * SCALE_X;
		v[2][2] = ((y+1) - pMap->sizeY/2) * SCALE_Y;

		// Get the normal for this face
		GetFaceNormal(v[0], v[1], v[2], temp);

		// Add normal of this face to the final normal
		norm[0] += temp[0];
		norm[1] += temp[1];
		norm[2] += temp[2];

		// Calculate image indices for face
		nIndex[0] = (y * pMap->sizeX + x) * 3;
		nIndex[1] = (y * pMap->sizeX + x - 1) * 3;
		nIndex[2] = ((y+1) * pMap->sizeX + x - 1) * 3;

		// Calculate vertices for face
		for (c=0;c<3;c++)
		{
			v[c][0] = (x - pMap->sizeX/2) * SCALE_X;
			v[c][2] = (y - pMap->sizeY/2) * SCALE_Y;
			v[c][1] = pMap->data[nIndex[c]] * SCALE_Z;
		}

		// Calculate differing vertices for other 2 verts
		v[1][0] = ((x-1) - pMap->sizeX/2) * SCALE_X;
		v[2][2] = ((y+1) - pMap->sizeY/2) * SCALE_Y;
		v[2][0] = ((x-1) - pMap->sizeX/2) * SCALE_X;

		// Get the normal for this face
		GetFaceNormal(v[0], v[1], v[2], temp);

		// Add normal of this face to the final normal
		norm[0] += temp[0];
		norm[1] += temp[1];
		norm[2] += temp[2];
	}

	// Get face normal for the upper left face
	if (x > 0 && y > 0)
	{
		// Calculate image indices for face
		nIndex[0] = ((y-1) * pMap->sizeX + x) * 3;
		nIndex[1] = (y * pMap->sizeX + x - 1) * 3;
		nIndex[2] = (y * pMap->sizeX + x) * 3;

		// Calculate vertices for face
		for (c=0;c<3;c++)
		{
			v[c][0] = (x - pMap->sizeX/2) * SCALE_X;
			v[c][2] = (y - pMap->sizeY/2) * SCALE_Y;
			v[c][1] = pMap->data[nIndex[c]] * SCALE_Z;
		}

		// Calculate differing vertices for other 2 verts
		v[0][2] = ((y-1) - pMap->sizeY/2) * SCALE_Y;
		v[1][0] = ((x-1) - pMap->sizeX/2) * SCALE_X;

		// Get the normal for this face
		GetFaceNormal(v[0], v[1], v[2], temp);

		// Add normal of this face to the final normal
		norm[0] += temp[0];
		norm[1] += temp[1];
		norm[2] += temp[2];
	}

	// Get face normal for the upper right faces
	if (x < pMap->sizeX - 1 && y > 0)
	{
		// Calculate image indices for face
		nIndex[0] = ((y-1) * pMap->sizeX + x + 1) * 3;
		nIndex[1] = (y*pMap->sizeX + x) * 3;
		nIndex[2] = (y * pMap->sizeX + x + 1) * 3;

		// Calculate vertices for face
		for (c=0;c<3;c++)
		{
			v[c][0] = (x - pMap->sizeX/2) * SCALE_X;
			v[c][2] = (y - pMap->sizeY/2) * SCALE_Y;
			v[c][1] = pMap->data[nIndex[c]] * SCALE_Z;
		}

		// Calculate differing vertices for other 2 verts
		v[0][0] = ((x+1) - pMap->sizeX/2) * SCALE_X;
		v[0][2] = ((y-1) - pMap->sizeY/2) * SCALE_Y;
		v[2][0] = ((x+1) - pMap->sizeX/2) * SCALE_X;

		// Get the normal for this face
		GetFaceNormal(v[0], v[1], v[2], temp);

		// Add normal of this face to the final normal
		norm[0] += temp[0];
		norm[1] += temp[1];
		norm[2] += temp[2];

		// Calculate image indices for face
		nIndex[0] = ((y-1) * pMap->sizeX + x) * 3;
		nIndex[1] = (y*pMap->sizeX + x) * 3;
		nIndex[2] = ((y-1) * pMap->sizeX + x + 1) * 3;

		// Calculate vertices for face
		for (c=0;c<3;c++)
		{
			v[c][0] = (x - pMap->sizeX/2) * SCALE_X;
			v[c][2] = (y - pMap->sizeY/2) * SCALE_Y;
			v[c][1] = pMap->data[nIndex[c]] * SCALE_Z;
		}

		// Calculate differing vertices for other 2 verts
		v[0][2] = ((y-1) - pMap->sizeY/2) * SCALE_Y;
		v[2][2] = ((y-1) - pMap->sizeY/2) * SCALE_Y;
		v[2][0] = ((x+1) - pMap->sizeX/2) * SCALE_X;

		// Get the normal for this face
		GetFaceNormal(v[0], v[1], v[2], temp);

		// Add normal of this face to the final normal
		norm[0] += temp[0];
		norm[1] += temp[1];
		norm[2] += temp[2];
	}

	// Normalize normal back to a unit length of 1.
	float len = sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);

	norm[0]/=len;
	norm[1]/=len;
	norm[2]/=len;
}

// LoadTexture - Loads a texture from a file and returns the texture ID
// for that texture.
GLuint LoadTexture(const char* filename)
{
	AUX_RGBImageRec* pTextureMap;
	GLuint nTex;

	// Load image
	pTextureMap = auxDIBImageLoad(filename);

	// Generate new Texture ID
	glGenTextures(1, &nTex);

	// Bind the texture ID
	glBindTexture(GL_TEXTURE_2D, nTex);

	// Specify linear MIN and MAG filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Clamp textures (no wrapping)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Specify image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, pTextureMap->sizeX,
		pTextureMap->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pTextureMap->data);

	// Image data is no longer needed, clean it up
	delete [] pTextureMap->data;
	delete pTextureMap;

	g_vecTextures.push_back(nTex);
	return nTex;
}

// LoadLandscape - Reads in a grayscale heightmap image, and a texture image
// and creates a display list for the landscape.
void LoadLandscape(const char* heightMap, const char* texture)
{
	AUX_RGBImageRec* pHeightMap;
	GLuint nLandscape;
	// Solid ambient & diffuse material
	float amb_diffuse[] = {1.0, 1.0, 1.0, 1.0};

	// Load Heightmap
	pHeightMap = auxDIBImageLoad(heightMap);

	// Load texture
	nLandscape = LoadTexture(texture);

	// Get new display list ID
	g_nLand = glGenLists(1);

	// Start new display list
	glNewList(g_nLand, GL_COMPILE);

	
	// Specify landscape texture
	glBindTexture(GL_TEXTURE_2D, nLandscape);
	// Set material
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, amb_diffuse);

	// Modulate texture with material
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Draw series of triangle strips
	for (int y=0;y<pHeightMap->sizeY - 1;y++)
	{
		// Start a new strip
		glBegin(GL_TRIANGLE_STRIP);
		for (int x=0;x<pHeightMap->sizeX;x++)
		{
			float norm[3];
			float pos[3];
			float tex[2];
			// Get index into image for this point
			int nIndex = (y*pHeightMap->sizeX + x) * 3;

			// Calculate texture coordinates
			tex[0] = static_cast<float>(x)/static_cast<float>(pHeightMap->sizeX - 1);
			tex[1] = static_cast<float>(y)/static_cast<float>(pHeightMap->sizeY - 1);
			glTexCoord2fv(tex);

			// Calculate the normal
			GetNormal(pHeightMap, x, y, norm);
			glNormal3fv(norm);

			// Calculate position
			pos[0] = (x - pHeightMap->sizeX/2) * SCALE_X;
			pos[1] = pHeightMap->data[nIndex] * SCALE_Z;
			pos[2] = (y - pHeightMap->sizeY/2) * SCALE_Y;
			glVertex3fv(pos);

			// Get index for point in image just below this one
			nIndex = ((y+1)*pHeightMap->sizeX + x)*3;

			// Calculate texture coordinates
			tex[0] = static_cast<float>(x)/static_cast<float>(pHeightMap->sizeX - 1);
			tex[1] = static_cast<float>(y+1)/static_cast<float>(pHeightMap->sizeY - 1);
			glTexCoord2fv(tex);

			// Calculate the normal
			GetNormal(pHeightMap, x, y+1, norm);
			glNormal3fv(norm);

			// Calculate the position
			pos[0] = (x - pHeightMap->sizeX/2) * SCALE_X;
			pos[1] = pHeightMap->data[nIndex] * SCALE_Z;
			pos[2] = ((y+1) - pHeightMap->sizeY/2) * SCALE_Y;
			glVertex3fv(pos);

		}
		// End current strip
		glEnd();
	}

	// End display list compilation.
	glEndList();

	
	delete [] pHeightMap->data;
	delete pHeightMap;
}

// LoadWater - Reads in a text file which specifies locations of water planes
// and textures which should be associated with those planes.  The planes are
// stored in a display list.
void LoadWater(const char* filename)
{
	// Use a white material with a slight transparent factor
	float amb_diffuse[] = {1.0, 1.0, 1.0, 0.6};

	// Open file
	ifstream fin(filename);

	if (fin.is_open())
	{
		// Generate new display list ID
		g_nWater = glGenLists(1);

		// Start new list
		glNewList(g_nWater, GL_COMPILE);
		// Specify material
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, amb_diffuse);
		// Turn off depth writing
		glDepthMask(false);
		while (!fin.eof())
		{
			GLuint texID;
			float texCoord[4][2];
			float pt[4][3];

			char texture[255];

			// Read texture filename
			if (fin >> texture)
			{
				// If the filename was not "" then try to get the
				// texture coordinates and plane position
				if (strlen(texture) > 0)
				{
					// assume success
					bool bSuccess = true;
					// Load texture
					texID = LoadTexture(texture);

					// should have 4 vertices
					for (int c=0;c<4;c++)
					{
						// Try to read texture coordinates
						if (!(fin >> texCoord[c][0] >> texCoord[c][1]))
						{
							bSuccess = false;
							break;
						}

						// Try to read plane position
						if (!(fin >> pt[c][0] >> pt[c][1] >> pt[c][2]))
						{
							bSuccess = false;
							break;
						}
					}

					// If everything loaded in fine, compile the plane into the list
					if (bSuccess)
					{
							// bind the texture
							glBindTexture(GL_TEXTURE_2D, texID);

							// Create a triangle strip for the water plane
							glBegin(GL_TRIANGLE_STRIP);
							{
								for (int c=0;c<4;c++)
								{
									glTexCoord2fv(texCoord[c]);
									glVertex3fv(pt[c]);
								}
							}
							// end triangle strip
							glEnd();
					}
				}
			}
		}

		// close the file
		fin.close();
		// Turn depth writing back on
		glDepthMask(true);
		// End display list compilation
		glEndList();
	}
}

// LoadSpray - Reads parameters for particle effect spray from a file and
// stores particles into a display list
void LoadSpray(const char* filename)
{
	// Light gray, 1/2 alpha value particles
	float amb_diffuse[] = {0.9, 0.9, 0.9, 0.5};
	int nDensity = 0;
	float size[3] = {1.0, 1.0, 1.0};
	float pos[3] = {0.0, 0.0, 0.0};
	float fRot;

	// Open file
	ifstream fin(filename);

	if (fin.is_open())
	{
		// Read in density
		fin >> nDensity;
		// Read in bounding box size
		fin >> size[0] >> size[1] >> size[2];
		// Read in position of particle box
		fin >> pos[0] >> pos[1] >> pos[2];
		// Read in Y-axis rotation of particles
		fin >> fRot;
		// close the file
		fin.close();
	}

	// Generate new display list id 
	g_nSpray = glGenLists(1);

	// Start a new list
	glNewList(g_nSpray, GL_COMPILE);
	// Specify material properties
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, amb_diffuse);
	// Don't use any textures for particles
	glBindTexture(GL_TEXTURE_2D, 0);
	// Turn off depth writing
	glDepthMask(false);
	// Position bounding box for particles
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(fRot, 0.0, 1.0, 0.0);

	// Start drawing random particles
	for (int c=0;c<nDensity;c++)
	{
		float pt[3];
		pt[0] = size[0] * static_cast<float>(rand() % 200)/200.0;
		pt[1] = size[1] * static_cast<float>(rand() % 200)/200.0;
		pt[2] = size[2] * static_cast<float>(rand() % 200)/200.0;

		// Particles near the top are smaller than those near the bottom
		glPointSize(5.0 * (size[1] - pt[1])/size[1]);
		glBegin(GL_POINTS);
		glVertex3fv(pt);
		glEnd();
	}
	glPopMatrix();
	// Turn depth writing back on
	glDepthMask(true);
	// End display list compilation
	glEndList();

}

// Init - Initialization function to initialize common OpenGL parameters
void Init()
{
	// ambient lighting color
	float amblight[] = {0.3, 0.3, 0.3, 1.0};

	// Specify clear color
	glClearColor(0.0, 0.0, 0.4, 1.0);
	glEnable(GL_DEPTH_TEST);		// Enable depth testing
	glEnable(GL_LIGHTING);			// Enable lighting
	glEnable(GL_LIGHT0);			// Enable Light 0
	glEnable(GL_TEXTURE_2D);		// Enable 2d texturing
	glEnable(GL_BLEND);				// Enable blending
	glEnable(GL_CULL_FACE);			// Enable backface culling
	
	// Hints for nicer rendering
#ifdef WIN32
	// Linux slows down too much when using these
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
#endif

	// Specify ambient light color
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amblight);
	// Specify blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load background texture, landscape, water, and particles
#ifdef WIN32
	g_nBackgroundTexture = LoadTexture("Background.bmp");
	LoadLandscape("height.bmp", "texture.bmp");
#else
	g_nBackgroundTexture = LoadTexture("Background.ppm");
	LoadLandscape("height.ppm", "texture.ppm");
#endif

#ifdef WIN32
	LoadWater("waterwin.txt");
#else
	LoadWater("waterlinux.txt");
#endif
	LoadSpray("particles.txt");
}

// PositionCamera - Repositions the camera
void PositionCamera()
{
	glLoadIdentity();
	gluLookAt(g_pCameraPos[0], g_pCameraPos[1], g_pCameraPos[2],
		g_pCameraOrient[0], g_pCameraOrient[1], g_pCameraOrient[2],
		0.0, 1.0, 0.0);
}

// SetFrustum - Setup the frustum properties
void SetFrustum()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fCameraFOV, static_cast<float>(g_uWidth)/static_cast<float>(g_uHeight),
		10.0, 250.0);

	glMatrixMode(GL_MODELVIEW);
}

// DrawBackground - Switches to an Orthographic projection, draws a textured quad
// using the background texture, then resets the frustum projection and camera positioning
void DrawBackground()
{
	// Calculate height
	float height = static_cast<float>(g_uWidth)/static_cast<float>(g_uHeight);
	// Use pure white material with no transparency
	float amb_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	
	// Change to orthographic projection to easily draw over whole scene
	// with the background texture.
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -height, height, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Disable depth test
	glDisable(GL_DEPTH_TEST);
	// Setup material and set texture mode to replace
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, amb_diffuse);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Bind background texture and draw quad over whole screen
	glBindTexture(GL_TEXTURE_2D, g_nBackgroundTexture);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0, height);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-1.0, -height);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(1.0, -height);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(1.0, height);
	}
	glEnd();
	// Re-enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Reset frustum and camera position
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// OnDisplay - Callback function that is called everytime the window
// needs to be re-painted
void OnDisplay()
{
	// Position light
	float lightpos[] = {0.0, 0.71, 0.71, 0.0};

	// Draw the background. Note that since the background
	// is to be drawn over the whole window regardless of
	// depth, clearing the frame buffer is not necessary
	// unless we are in wireframe mode.
	if (g_bWireframe)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_DEPTH_BUFFER_BIT);

	DrawBackground();
	
	// Rotate the scene as needed
	glPushMatrix();
	glRotatef(g_fLandRot, 0.0, 1.0, 0.0);
	// Position the light
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	// Call display lists
	glCallList(g_nLand);
	glCallList(g_nWater);
	glCallList(g_nSpray);
	glPopMatrix();

	// Swap buffers.  Double buffering is used to avoid animation tearing.
	glutSwapBuffers();
}

// OnReshape - Callback function that is called when window is initially created
// and whenever the window is resized after that.
void OnReshape(int width, int height)
{
	// Reset the window viewport
	glViewport(0, 0, width, height);
	g_uHeight = height;
	g_uWidth = width;

	// Reset the frustum and camera position
	SetFrustum();
	PositionCamera();

	// Redisplay the scene
	glutPostRedisplay();
}

// OnKeyboard - Callback function that is called whenever a key is pressed
void OnKeyboard(unsigned char key, int xPos, int yPos)
{
	float vecForward[3];
	float vecSide[3];
	float len, x, y, z;

	// Calculate forward vector
	for (int c=0;c<3;c++)
	{
		vecForward[c] = g_pCameraOrient[c] - g_pCameraPos[c];
	}

	// Normalize forward vector
	len = sqrt(vecForward[0]*vecForward[0] + vecForward[1]*vecForward[1] + vecForward[2]*vecForward[2]);
	vecForward[0] /= len;
	vecForward[1] /= len;
	vecForward[2] /= len;

	// Calculate side vector
	vecSide[0] = -vecForward[2];
	vecSide[1] = 0.0;
	vecSide[2] = vecForward[0];

	// Normalize side vector
	len = sqrt(vecForward[0]*vecForward[0] + vecForward[1]*vecForward[1] + vecForward[2]*vecForward[2]);
	vecSide[0] /= len;
	vecSide[1] /= len;
	vecSide[2] /= len;

	// Calculate offset of the camera view orientation from the position
	x = g_pCameraOrient[0] - g_pCameraPos[0];
	y = g_pCameraOrient[1] - g_pCameraPos[1];
	z = g_pCameraOrient[2] - g_pCameraPos[2];

	switch(key)
	{
	// Zoom (Increase/decrease frustum's FOV)
	case 'B':
	case 'b':
		g_fCameraFOV += 1;
		break;
	case 'V':
	case 'v':
		g_fCameraFOV -= 1;
		break;

	// Forward/back movement
	case 'W':
	case 'w':
		g_pCameraPos[0] += vecForward[0];
		g_pCameraPos[1] += vecForward[1];
		g_pCameraPos[2] += vecForward[2];
		g_pCameraOrient[0] += vecForward[0];
		g_pCameraOrient[1] += vecForward[1];
		g_pCameraOrient[2] += vecForward[2];
		break;
	case 'S':
	case 's':
		g_pCameraPos[0] -= vecForward[0];
		g_pCameraPos[1] -= vecForward[1];
		g_pCameraPos[2] -= vecForward[2];
		g_pCameraOrient[0] -= vecForward[0];
		g_pCameraOrient[1] -= vecForward[1];
		g_pCameraOrient[2] -= vecForward[2];
		break;

	// Turn left/right
	case 'Q':
	case 'q':
		g_pCameraOrient[0] = x*COS1 + z*SIN1 + g_pCameraPos[0];
		g_pCameraOrient[2] = -x*SIN1 + z*COS1 + g_pCameraPos[2];
		break;
	case 'E':
	case 'e':
		g_pCameraOrient[0] = x*COS1 - z*SIN1 + g_pCameraPos[0];
		g_pCameraOrient[2] = x*SIN1 + z*COS1 + g_pCameraPos[2];
		break;

	// Look up/down.  This is a sloppy way of doing it but the effect
	// is good enough for this program.
	case 'R':
	case 'r':
		g_pCameraOrient[1] += 1.0;
		break;
	case 'F':
	case 'f':
		g_pCameraOrient[1] -= 1.0;
		break;

	// Move the camera up/down
	case 'T':
	case 't':
		g_pCameraPos[1] += 1.0;
		g_pCameraOrient[1] += 1.0;
		break;
	case 'G':
	case 'g':
		g_pCameraPos[1] -= 1.0;
		g_pCameraOrient[1] -= 1.0;
		break;

	// Strafe left/right
	case 'A':
	case 'a':
		g_pCameraPos[0] -= vecSide[0];
		g_pCameraPos[1] -= vecSide[1];
		g_pCameraPos[2] -= vecSide[2];
		g_pCameraOrient[0] -= vecSide[0];
		g_pCameraOrient[1] -= vecSide[1];
		g_pCameraOrient[2] -= vecSide[2];
		break;
	case 'D':
	case 'd':
		g_pCameraPos[0] += vecSide[0];
		g_pCameraPos[1] += vecSide[1];
		g_pCameraPos[2] += vecSide[2];
		g_pCameraOrient[0] += vecSide[0];
		g_pCameraOrient[1] += vecSide[1];
		g_pCameraOrient[2] += vecSide[2];
		break;

	// Rotate land left/right
	case 'z':
	case 'Z':
		g_fLandRot += 2.0;
		break;
	case 'c':
	case 'C':
		g_fLandRot -= 2.0;
		break;
	case 'l':
		g_bWireframe = true;
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 'L':
		g_bWireframe = false;
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	}
	SetFrustum();
	PositionCamera();
	glutPostRedisplay();
}

// OnIdle - Callback function that is called whenever nothing else is being done.
void OnIdle()
{
	// Redisplay the scene.  This is not really necessary for this project as there
	// is no animation, but if animation were to be added this would be a good spot
	// to advance the animation parameters and redisplay the scene.
	//glutPostRedisplay();
}

#ifndef WIN32
// auxDIBImageLoad - Since Linux doesn't have the glAux libraries and headers
// we have to handle image loading ourselves.  This function just reads in
// ASCII formatted PPM files.
AUX_RGBImageRec* auxDIBImageLoad(const char* filename)
{
	char buffer[256];
	ifstream fin(filename);
	AUX_RGBImageRec* pImg = new AUX_RGBImageRec;
	int nMax;
	
	if (fin.is_open())
	{
		// Discard first 2 lines.  Can check the first
		// line to validate the format.
		fin.getline(buffer, 255);
		// Second line is just a comment
		fin.getline(buffer, 255);
		
		fin >> pImg->sizeX >> pImg->sizeY >> nMax;
				
		// The following is for ASCII format PPMs only
		// to use this for binary PPMs, you need to set the
		// ifstream mode to not skip whitespace and to read binary.
		// You then would read in one unsigned char at a time.
		
		pImg->data = new GLubyte[pImg->sizeX * pImg->sizeY * 3]; 
		
		for (int y=0;y<pImg->sizeY;y++)
		{
			for (int x=0;x<pImg->sizeX;x++)
			{
				int nIndex = ((pImg->sizeY-y-1)*pImg->sizeX + x)*3;
				int nRed, nGreen, nBlue;
				
				if (fin >> nRed >> nGreen >> nBlue)
				{
					pImg->data[nIndex] = nRed;
					pImg->data[nIndex+1] = nGreen;
					pImg->data[nIndex+2] = nBlue;
				}
								
			}
		}
		fin.close();
	}

	return pImg;
}
#endif
