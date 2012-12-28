///////////////////////////////////////////////////////////
// Created: 11/18/2000
// Last Modified: 11/19/2000
//
// This program simulates a tornado.  It does so by rotating
// points around a Bezier curve.  The control points of the
// Bezier curve are randomly moved about to generate a bending
// motion to the overall shape of the funnel cloud.
///////////////////////////////////////////////////////////
#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <string.h>
#include <list>

// Struct to hold information about dust particles
// stirred up by the tornado's tail.
struct DUST_PARTICLE
{
	int timeToLive;
	float pos[3];
	float vel[3];
};

// Declarations of functions
void Display();
void Resize(int w, int h);
void Idle();
void Init();

// Constants used to determine how things are displayed
const float PI = 3.141592654;
const int GRADIANTS = 100;
const float FUNNEL_SIZE = 3.0;
const int BLUR_FACTOR = 3;
const int DUST_DENSITY = 60;
const int DUST_LIFE = 15;
const float DUST_VELOCITY = 0.3;

// Container types for storing information about the tornado and dust particles
typedef std::vector<int> PtAngleList;
typedef std::vector<PtAngleList> PtAngleLists;
typedef std::list<DUST_PARTICLE*> DustCloud;

// Arrays to hold sin/cos tables so that the expensive sin/cos functions
// do not need to be called so often.
float g_pSinTable[360];
float g_pCosTable[360];

// Containers to store dust particles and points in the tornado
PtAngleLists g_listAngles(GRADIANTS);
DustCloud g_listDust;

// Globals to determine what is shown
bool g_bShowControlPoints = false;
bool g_bShowControlLinks = false;
bool g_bShowCore = false;

// Initial control points for the Bezier curve
float g_pControls[4][3] = 
{
	{0.0, -6.0, 0.0}, 
	{-3.0, -2.0, 0.0},
	{3.0, 2.0, 0.0},
	{0.0, 6.0, 0.0}
};

// Main entry point for the program
int main(int argc, char** argv)
{
	// Parse command-line arguments to determine what should be displayed
	for (int c=1;c<argc;c++)
	{
		if (strcmp(argv[c], "/showall") == 0)
		{
			g_bShowControlLinks = true;
			g_bShowControlPoints = true;
			g_bShowCore = true;
			break;
		}
		else if (strcmp(argv[c], "/showcore") == 0)
		{
			g_bShowCore = true;
		}
		else if (strcmp(argv[c], "/showpoints") == 0)
		{
			g_bShowControlPoints = true;
		}
		else if(strcmp(argv[c], "/showlines") == 0)
		{
			g_bShowControlLinks = true;
		}
	}

	// Seed the random number generator based on the current time
	srand(time(NULL));

	// Initialize the glut window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutCreateWindow("CS458 - Tornado");

	Init();

	// Register glut callback functions
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(Resize);

	// Start the main Glut window
	glutMainLoop();
	
	return 0;
}

// UpdateControlPoints() - This function randomly moves the control points.
// Each point is moved in a random direction for 10 frames, and then the
// direction for that point is changed again.
void UpdateControlPoints()
{
	// Static variables to hold current control point direction
	static float xd[4];
	static float zd[4];
	static int update = 0;

	// Remap the control points to the Bezier curve
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_pControls[0][0]);

	// If the time is up, select a new random direction for each point
	if (++update % 10 == 0)
	{
		for (int c=0;c<4;c++)
		{
			xd[c] = static_cast<float>(rand() % 100)/500.0 - 0.1;
			zd[c]= static_cast<float>(rand() % 100)/500.0 - 0.1;
		}
	}

	// Move the control points
	for (int c=0;c<4;c++)
	{
		g_pControls[c][0] += xd[c];
		g_pControls[c][2] += zd[c];
	}
}

// Display() - General rendering function to render the tornado, control-points,
// Bezier curve core, and interconnecting lines for the control points.
void Display()
{
	float xPos, zPos;

	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the points BLUR_FACTOR times in order to create the
	// feel of motion for each point in the tornado.
	for (int mot=0;mot<BLUR_FACTOR;mot++)
	{
		// Determine a color to use for the tornado based on which rendering in
		// the motion blur loop this is.
		float color = (static_cast<float>(BLUR_FACTOR-mot)/BLUR_FACTOR) * 0.75;
		
		glColor3f(color, color, color);

		// Draw the tornado form the bottom of the tail up
		for (int c=0;c<GRADIANTS;c++)
		{
			// Determine the radius of the tornado at this height
			float len = (static_cast<float>(c)/GRADIANTS) * FUNNEL_SIZE;

			// Draw the points for this level of the tornado
			for (int d=0;d<=c;d++)
			{
				// Determine the offset of the bezier curve for this point
				xPos = g_pSinTable[g_listAngles[c][d]] * len;
				zPos = g_pCosTable[g_listAngles[c][d]] * len;
				
				// Advance the angle of the currently drawn point by 1 degree
				g_listAngles[c][d] = (g_listAngles[c][d] + 1) % 360;

				// Push the current matrix on the stack, then translate according
				// to the calculated x and z offset for the current point
				glPushMatrix();
				glTranslatef(xPos, 0.0, 0.0);
				glTranslatef(0.0, 0.0, zPos);

				// Draw the point of the bezier curve at the current height
				glBegin(GL_POINTS);
				{
					glEvalCoord1f(static_cast<float>(c)/GRADIANTS);
				}
				glEnd();
				
				// Return the state of the matrix to what it was before
				// translating the offsets for this point
				glPopMatrix();
				
			}
		}
	}

	// Draw the dust cloud.
	DustCloud::iterator itr;

	// Set the color of the dust cloud points to a brown with a 70% alpha component
	glColor4f(0.6235, 0.3686, 0.1412, 0.7);

	// Begin drawing the points
	glBegin(GL_POINTS);

	// Iterate through the list of dust particles
	itr=g_listDust.begin();
	while (itr!=g_listDust.end())
	{
		DUST_PARTICLE* pDust = *itr;

		// Draw the particle
		glVertex3fv(pDust->pos);

		// Decrement the dust particles length to live
		pDust->timeToLive--;

		if (pDust->timeToLive > 0)
		{
			// Adjust the location of the dust particle based on it's velocity
			pDust->pos[0] += pDust->vel[0];
			pDust->pos[1] += pDust->vel[1];
			pDust->pos[2] += pDust->vel[2];
			itr++;
		}
		else
		{
			// The life of this dust particle has expired
			// remove it from the list and free it's memory
			delete pDust;
			itr = g_listDust.erase(itr);
		}
	}
	glEnd();

	// Add new dust particles to the list
	for (int dust=0;dust<DUST_DENSITY;dust++)
	{
		DUST_PARTICLE* pDust = new DUST_PARTICLE;
		float len;

		// Set current position to the tail of the tornado
		pDust->timeToLive = DUST_LIFE;
		pDust->pos[0] = g_pControls[0][0];
		pDust->pos[1] = g_pControls[0][1];
		pDust->pos[2] = g_pControls[0][2];

		// Calculate a random direction/velocity vector for the particle
		pDust->vel[0] = (rand() % 100) - 50;
		pDust->vel[1] = (rand() % 100) + 15;
		pDust->vel[2] = (rand() % 100) - 50;

		// Adjust the speed of the particle
		len = sqrt(pDust->vel[0]*pDust->vel[0] + pDust->vel[1]*pDust->vel[1] +
			pDust->vel[2]*pDust->vel[2]);
		len = (DUST_VELOCITY / len);

		pDust->vel[0] *= len;
		pDust->vel[1] *= len;
		pDust->vel[2] *= len;

		// Add a small random factor to the speed to prevent the particles
		// from appearing layered to the human eye.
		pDust->vel[0] += static_cast<float>(rand() % 100)/1000.0;
		pDust->vel[1] += static_cast<float>(rand() % 100)/1000.0;
		pDust->vel[2] += static_cast<float>(rand() % 100)/1000.0;

		// Add the particle to the list
		g_listDust.push_back(pDust);
	}

	// If the control points are to be shown, draw them now
	if (g_bShowControlPoints)
	{
		glPointSize(5.0);
		glColor3f(1.0, 0.0, 0.0);

		glBegin(GL_POINTS);
		for (int pt=0;pt<4;pt++)
		{
			glVertex3fv(g_pControls[pt]);
		}
		glEnd();
		glPointSize(1.5);

	}

	// If the lines interconnecting the control points are to be shown, draw them
	if (g_bShowControlLinks)
	{
		glColor3f(1.0, 1.0, 0.0);

		glBegin(GL_LINE_STRIP);
		for (int pt=0;pt<4;pt++)
		{
			glVertex3fv(g_pControls[pt]);
		}
		glEnd();
	}

	// If the Bezier curve "core" of the tornado is to be shown, display it
	if (g_bShowCore)
	{
		glColor3f(0.0, 0.0, 1.0);

		glBegin(GL_LINE_STRIP);
		for (int pt=0;pt<GRADIANTS;pt++)
		{
			glEvalCoord1f(static_cast<float>(pt)/GRADIANTS);
		}
		glEnd();
	}

	// Swap the back and front buffers to display the scene
	glutSwapBuffers();

	// Update the control points.
	UpdateControlPoints();
}

// Resize() - Function to handle resizing of the window.  This function
// sets up the OpenGL viewport and projection matrix.  Since it is always
// called at least once when the window is created, we don't have to 
// set these things elsewhere.
void Resize(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-7, 7, -7, 7, 20, 40);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -30.0);

	glutPostRedisplay();
}

// InitializePoints() - This function sets up the initial points for the
// tornado.  They are randomly placed at different angles around the core
// Bezier curve.
void InitializePoints()
{
	for (int c=0;c<GRADIANTS;c++)
	{
		g_listAngles[c].resize(c+1);

		for (int d=0;d<=c;d++)
		{
			g_listAngles[c][d] = rand() % 360;
		}
	}
}

// SetupTrigTables() - Sets up the sin and cos tables.  Using these tables is
// somewhat more efficient than calling the sin/cos functions
void SetupTrigTables()
{
	for (int c=0;c<360;c++)
	{
		float fAngle = static_cast<float>(c)*PI/180.0;

		g_pSinTable[c] = sin(fAngle);
		g_pCosTable[c] = cos(fAngle);
	}
}

// Init() - Initializes some of the OpenGL settings and then sets up the 
// sin/cos tables and Initializes the points used for the tornado
void Init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_MAP1_VERTEX_3);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	SetupTrigTables();
	UpdateControlPoints();
	InitializePoints();

	glPointSize(1.5);
	glColor3f(0.0, 0.0, 0.0);
}

// Idle() - Forces the window to be refreshed for animation purposes.
void Idle()
{
	glutPostRedisplay();
}
