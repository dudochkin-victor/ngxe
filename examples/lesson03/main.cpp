/*
 NeHe (nehe.gamedev.net) OpenGL tutorial series
 GLUT port.in 2001 by milix (milix_gr@hotmail.com)
 Most comments are from the original tutorials found in NeHe.
 For VC++ users, create a Win32 Console project and link 
 the program with glut32.lib, glu32.lib, opengl32.lib
 */

#include <QApplication>
#include "qmlapplicationviewer.h"
#include <stdio.h>
#include <Plane.h>
#include <Vector2D.h>
#include <Vector3D.h>
#include <Matrix3D.h>
#include <BoundingBox.h>
#include <Camera.h>
#include <noise.h>
#include <Frustum.h>
#include <GlslProgram.h>
#include <libTexture.h>
#include "View.h"
#include "Util.h"

using namespace std;
										// size of height map
#define	N1	128
#define	N2	128

Vector3D	vertex [N1][N2];			// vertices of a heightmap

struct	GrassVertex
{
	Vector3D	pos;
	Vector3D	tex;					// z component is stiffness
	Vector3D	refPoint;
	Vector3D	refColor;
};

struct	GrassObject
{
	Vector3D	point;					// ref point
	Vector3D	color;					// ref color
	GrassVertex	vertex [4*3];
};

class	Cell							// basic bulding block for landscape
{
protected:
	list <GrassObject *> grass;
	unsigned			 vertexId;
	unsigned			 indexId;
	BoundingBox			 bounds;
	int					 x, y;
	int					 xSize, ySize;

public:
	Cell  ( int theX, int theY, int dx, int dy );
	~Cell ();

	const BoundingBox&	getBounds () const
	{
		return bounds;
	}

	void	initObjects ( const list<GrassObject *>& objects );
	void	draw        ( const Camera& camera );
};

Cell :: Cell ( int theX, int theY, int dx, int dy ) : x ( theX ), y ( theY ), xSize ( dx ), ySize ( dy )
{
	vertexId = 0;
	indexId  = 0;
}

Cell :: ~Cell ()
{
	if ( vertexId != 0 )
		glDeleteBuffersARB ( 1, &vertexId );

	if ( indexId != 0 )
		glDeleteBuffersARB ( 1, &indexId  );
}

void	Cell :: initObjects ( const list<GrassObject *>& objects )
{
	static	int vertexStride = sizeof ( GrassVertex );

											// collect all grass objects within this cell
	for ( list <GrassObject *> :: const_iterator it = objects.begin (); it != objects.end (); ++it )
	{
		GrassObject * object = *it;

		for ( int i = 0; i < 12; i++ )
			if ( object -> vertex [i].pos.x >= x && object -> vertex [i].pos.x < x + xSize &&
			     object -> vertex [i].pos.y >= y && object -> vertex [i].pos.y < y + ySize )
			{
				grass.push_front ( object );

				break;
			}
	}

	if ( grass.empty () )
		return;
											// update bounds with list and prepare vertex buffer
	int			  numVertices = grass.size () * 12;
	GrassVertex * vertices    = new GrassVertex [numVertices];
	int			  count       = 0;

	for ( list <GrassObject *> :: iterator gi = grass.begin (); gi != grass.end (); ++gi )
	{
		GrassObject * object = *gi;

		for ( int i = 0; i < 12; i++ )
		{
			bounds.addVertex ( object -> vertex [i].pos );
			vertices [count++] = object -> vertex [i];
		}
	}
											// create vertex buffer
    glGenBuffersARB ( 1, &vertexId );
    glBindBufferARB ( GL_ARRAY_BUFFER_ARB, vertexId );
    glBufferDataARB ( GL_ARRAY_BUFFER_ARB, numVertices * sizeof ( GrassVertex ), vertices, GL_STREAM_DRAW_ARB );
    glBindBufferARB ( GL_ARRAY_BUFFER_ARB, 0 );

	delete vertices;
}

void	Cell :: draw ( const Camera& camera)
{
	static	GrassVertex	_v;
	static	int 		vertexStride = sizeof ( GrassVertex );

	static	int offsets [] =
	{
		((uint64_t)&_v.pos)      - ((uint64_t)&_v),	//  position
		((uint64_t)&_v.tex)      - ((uint64_t)&_v),	// texture coordinates
		((uint64_t)&_v.refPoint) - ((uint64_t)&_v),	// reference point (as texCoord[1])
		((uint64_t)&_v.refColor) - ((uint64_t)&_v)	// reference color (as vec3)
	};

//	float	distSq = camera.getPos ().distanceToSq ( bounds.getMinPoint () );

//	if ( distSq <= 500 )
	{
												// save state
		glPushClientAttrib  ( GL_CLIENT_VERTEX_ARRAY_BIT );

												// setup vertex buffer
		glBindBufferARB          ( GL_ARRAY_BUFFER_ARB, vertexId );

		glEnableClientState      ( GL_VERTEX_ARRAY );
		glVertexPointer          ( 3, GL_FLOAT, vertexStride, (void *) offsets [0] );

		glEnableClientState      ( GL_COLOR_ARRAY );
		glColorPointer           ( 3, GL_FLOAT, vertexStride, (void *) offsets [3] );

		glClientActiveTextureARB ( GL_TEXTURE0_ARB );
		glEnableClientState      ( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer        ( 3, GL_FLOAT, vertexStride, (void *) offsets [1] );

		glClientActiveTextureARB ( GL_TEXTURE1_ARB );
		glEnableClientState      ( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer        ( 3, GL_FLOAT, vertexStride, (void *) offsets [2] );

		glDrawArrays ( GL_QUADS, 0, grass.size () * 12 );

													// unbind array buffer
		glBindBufferARB ( GL_ARRAY_BUFFER_ARB,  0 );

		glPopClientAttrib ();
	}
}

#define	CELL_SIZE	16
#define	NC1			(N1/CELL_SIZE)
#define	NC2			(N2/CELL_SIZE)

list<GrassObject *>	grass;
Cell * cells [NC1][NC2];

Vector3D    eye   ( 0, 0, 10 );  		// camera position
unsigned    decalMap;                   // decal (diffuse) texture
unsigned	grassMap;
unsigned    stoneMap;
unsigned    teapotMap;
unsigned	noiseMap;
float       angle     = 0;

float	yaw   = 0;
float	pitch = 0;
float	roll  = 0;

Camera			camera ( eye, 0, 0, 0 );	// camera to be used
Noise			noise;
GlslProgram 	program;
GlslProgram		program2;

inline	float	rnd ()
{
	return (float) rand () / (float) RAND_MAX;
}

inline	float rnd ( float x1, float x2 )
{
	return x1 + (x2 - x1) * rnd ();
}

inline float	heightFunc ( float x, float y )
{
	return 5 * ( 1.0 + noise.noise ( 0.09375268*Vector3D ( x, y, 0.12387 ) ) );
}

void	adjustCameraHeight ()
{
	Vector3D	pos = camera.getPos ();

	pos.z = heightFunc ( pos.x, pos.y ) + 1.4;

	camera.moveTo ( pos );
}

void	initLandscape ()
{
	for ( int i = 0; i < N1; i++ )
		for ( int j = 0; j < N2; j++ )
		{
			vertex [i][j].x = i - 0.5 * N1;
			vertex [i][j].y = j - 0.5 * N2;
			vertex [i][j].z = heightFunc ( vertex [i][j].x, vertex [i][j].y );
		}
}

void	buildGrassBase ( float start, float r, float delta, Vector3D * tri, Vector3D * gr )
{
	for ( int i = 0; i < 3; i++ )
	{
		tri [i].x = r * cos ( start + i * M_PI * 2.0 / 3.0 );
		tri [i].y = r * sin ( start + i * M_PI * 2.0 / 3.0 );
		tri [i].z = 0;
	}
											// create grass base
	gr [0] = tri [0] - Vector3D ( 0, delta, 0 );
	gr [1] = tri [1] - Vector3D ( 0, delta, 0 );
	gr [2] = tri [1] + Vector3D ( delta, 0, 0 );
	gr [3] = tri [2] + Vector3D ( delta, 0, 0 );
	gr [4] = tri [2] + Vector3D ( 0, delta, 0 );
	gr [5] = tri [0] + Vector3D ( 0, delta, 0 );
}

void	initGrass ( float averageDist )
{
											// now fill with grass objects
	Vector3D	tri [3];					// triangle base (with respect to O)
	Vector3D	gr  [6];					// grass base
	float		r        = 1.0;
	float		delta    = 0.3*r;
	float		upScale  = 1.0;
	float		texStart = 0.0;
	Vector3D 	up ( 0, 0, r );
	int			count = 0;
	int			line  = 0;
	Vector3D	c1 ( 0.55, 0.66, 0.2  );
	Vector3D	c2 ( 0,  0.8,    0.2 );
//Vector3D c1 ( 0, 1, 0 );
//Vector3D c2 ( 0, 0, 1 );

	for ( float x0 = -0.5*N1; x0 < 0.5*N1; x0 += averageDist, line++ )
		for ( float y0 = -0.5*N2; y0 < 0.5*N2; y0 += averageDist )
		{
			float	x     = x0 + ( rnd () - 0.5 ) * averageDist*1.2;
			float	y     = y0 + ( rnd () - 0.5 ) * averageDist*1.2;
			float	scale = rnd ( 0.8f, 1.2f );

			if ( line & 1 )
				x += 0.5 * averageDist;

			if ( ((count++) % 17) == 0 )
				buildGrassBase ( rnd (), r, delta, tri, gr );

			Vector3D	  point  = Vector3D ( x, y, heightFunc ( x, y ) );
			float		  t      = 0.5f * (1.0f + noise.noise ( 0.3968*Vector3D ( x, y + 0.173, 0.787 ) ) );
			Vector3D	  color  = t*c1 + (1-t)*c2;//lerp ( c1, c2, t );
			GrassObject * object = new GrassObject;
//printf ( "%f %f %f\n", color.x, color.y, color.z );

//			texStart = 0.25*(rand () % 4);
			up.x     = upScale*(rnd () - 0.5);
			up.y     = upScale*(rnd () - 0.5);

			object -> point = point;
			object -> vertex [0].pos = point + scale*gr [0];
			object -> vertex [0].tex = Vector3D ( 0, 0, 0 );
			object -> vertex [1].pos = point + scale*gr [1];
			object -> vertex [1].tex = Vector3D ( texStart + 0.25, 0, 0 );
			object -> vertex [2].pos = point + scale*gr [1] + scale*up;
			object -> vertex [2].tex = Vector3D ( texStart + 0.25, 1, 1 );
			object -> vertex [3].pos = point + scale*gr [0] + scale*up;
			object -> vertex [3].tex = Vector3D ( 0, 1, 1 );

//			texStart = 0.25*(rand () % 4);
			up.x     = upScale*(rnd () - 0.5);
			up.y     = upScale*(rnd () - 0.5);

			object -> vertex [4].pos = point + scale*gr [2];
			object -> vertex [4].tex = Vector3D ( 0, 0, 0 );
			object -> vertex [5].pos = point + scale*gr [3];
			object -> vertex [5].tex = Vector3D ( texStart + 0.25, 0, 0 );
			object -> vertex [6].pos = point + scale*gr [3] + scale*up;
			object -> vertex [6].tex = Vector3D ( texStart + 0.25, 1, 1 );
			object -> vertex [7].pos = point + scale*gr [2] + scale*up;
			object -> vertex [7].tex = Vector3D ( 0, 1, 1 );

//			texStart = 0.25*(rand () % 4);
			up.x     = upScale*(rnd () - 0.5);
			up.y     = upScale*(rnd () - 0.5);

			object -> vertex [8].pos  = point + scale*gr [3];
			object -> vertex [8].tex  = Vector3D ( 0, 0, 0 );
			object -> vertex [9].pos  = point + scale*gr [4];
			object -> vertex [9].tex  = Vector3D ( texStart + 0.25, 0, 0 );
			object -> vertex [10].pos = point + scale*gr [4] + scale*up;
			object -> vertex [10].tex = Vector3D ( texStart + 0.25, 1, 1 );
			object -> vertex [11].pos = point + scale*gr [3] + scale*up;
			object -> vertex [11].tex = Vector3D ( 0, 1, 1 );

			for ( int i = 0; i < 12; i++ )
			{
				object -> vertex [i].refPoint = point;
				object -> vertex [i].refColor = color;
			}

			grass.push_back ( object );
		}

	printf ( "Created %d grass objects\n", grass.size () );

											// now start bulding cells
	printf ( "Initializing cells\n" );

	for ( int i = 0; i < NC1; i++ )
		for ( int j = 0; j < NC2; j++ )
		{
			cells [i][j] = new Cell ( i * CELL_SIZE - 0.5 * N1, j * CELL_SIZE - 0.5 * N2, CELL_SIZE, CELL_SIZE );

			cells [i][j] -> initObjects ( grass );
		}

	printf ( "Cells: done\n" );
}

//
// draw a set of grass billboards near the camera with the fixed step h
//
void	drawGrassBillboards ( float h, float delta )
{
	float	x  = camera.getPos ().x;
	float	y  = camera.getPos ().y;
	float	x1 = x - delta;
	float	y1 = y - delta;
	float	x2 = x + delta;
	float	y2 = y + delta;
											// now get indices range
	int	i1 = x1 / h;
	int	j1 = y1 / h;
	int	i2 = x2 / h;
	int	j2 = y2 / h;

	program2.bind ();

	glBegin ( GL_QUADS );

	for ( int i = i1; i <= i2; i++ )
		for ( int j = j1; j <= j2; j++ )
		{
			Vector3D	point ( i * h, j * h, heightFunc ( i * h, j * h ) + 0.3 );

			glTexCoord3f ( 0,    0, 0 );
			glVertex3fv  ( point );
			glTexCoord3f ( 0.25, 0, 1 );
			glVertex3fv  ( point );
			glTexCoord3f ( 0.25, 1, 1 );
			glVertex3fv  ( point );
			glTexCoord3f ( 0,    1, 0 );
			glVertex3fv  ( point );
		}

	glEnd ();

	program2.unbind ();
}

void	drawGrass ()
{
	glActiveTextureARB ( GL_TEXTURE1_ARB );
	glBindTexture      ( GL_TEXTURE_2D, noiseMap );
	glActiveTextureARB ( GL_TEXTURE0_ARB );
	glBindTexture      ( GL_TEXTURE_2D, grassMap );
	glEnable           ( GL_TEXTURE_2D );
	glEnable           ( GL_ALPHA_TEST );
	glDisable          ( GL_BLEND      );
	glAlphaFunc        ( GL_GEQUAL, 0.5 );

	Frustum	frustum;

    program.bind ();

	glColor4f ( 1, 1, 1, 1 );

	for ( int i = 0; i < NC1; i++ )
		for ( int j = 0; j < NC2; j++ )
			if ( frustum.boxInFrustum ( cells [i][j] -> getBounds () ) )
				cells [i][j] -> draw ( camera );

    program.unbind ();

	drawGrassBillboards ( 0.4, 3 );

	glDisable  ( GL_ALPHA_TEST );
}

void init ()
{
    glClearColor ( 0.0, 0.0, 0.0, 1.0 );
    glEnable     ( GL_DEPTH_TEST );
    glEnable     ( GL_TEXTURE_2D );
    glDepthFunc  ( GL_LEQUAL     );

    glHint ( GL_POLYGON_SMOOTH_HINT,         GL_NICEST );
    glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}
void display ()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode ( GL_MODELVIEW );
	glPushMatrix ();

	camera.apply ();

	glBindTexture ( GL_TEXTURE_2D, decalMap );
	glEnable      ( GL_TEXTURE_2D );

	glColor4f ( 1, 1, 1, 1 );

	for ( int i = 0; i < N1 - 1; i++ )
		for ( int j = 0; j < N2 - 1; j++ )
		{
	      glBegin       ( GL_TRIANGLES );
			glTexCoord2f ( 0, 0 );
			glVertex3fv  ( vertex [i][j] );
			glTexCoord2f ( 1, 0 );
			glVertex3fv  ( vertex [i+1][j] );
			glTexCoord2f ( 1, 1 );
			glVertex3fv  ( vertex [i+1][j+1] );

			glTexCoord2f ( 0, 1 );
			glVertex3fv  ( vertex [i][j+1] );
			glTexCoord2f ( 1, 1 );
			glVertex3fv  ( vertex [i+1][j+1] );
			glTexCoord2f ( 0, 0 );
			glVertex3fv  ( vertex [i][j] );
	      glEnd ();
		}

	drawGrass ();

	glMatrixMode ( GL_MODELVIEW );
	glPopMatrix  ();

    glutSwapBuffers ();
}

void reshape ( int w, int h )
{
	camera.setViewSize ( w, h, 60 );
	camera.apply       ();
}


void key ( unsigned char key, int x, int y )
{
    if ( key == 27 || key == 'q' || key == 'Q' )        // quit requested
        exit ( 0 );
    else
   	if ( key == 'w' || key == 'W' )
   		camera.moveBy ( camera.getViewDir () * 0.2 );
   	else
   	if ( key == 'x' || key == 'X' )
   		camera.moveBy ( -camera.getViewDir () * 0.2 );
   	else
   	if ( key == 'a' || key == 'A' )
   		camera.moveBy ( -camera.getSideDir () * 0.2 );
   	else
   	if ( key == 'd' || key == 'D' )
   		camera.moveBy ( camera.getSideDir () * 0.2 );

	adjustCameraHeight ();
   	glutPostRedisplay  ();
}

void    specialKey ( int key, int x, int y )
{
    if ( key == GLUT_KEY_UP )
        yaw += M_PI / 90;
    else
    if ( key == GLUT_KEY_DOWN )
        yaw -= M_PI / 90;
	else
    if ( key == GLUT_KEY_RIGHT )
        roll += M_PI / 90;
    else
    if ( key == GLUT_KEY_LEFT )
        roll -= M_PI / 90;

	camera.setEulerAngles ( yaw, pitch, roll );

    glutPostRedisplay ();
}

void	mouseFunc ( int x, int y )
{
	static	int	lastX = -1;
	static	int	lastY = -1;

	if ( lastX == -1 )				// not initialized
	{
		lastX = x;
		lastY = y;
	}

	yaw  -= (y - lastY) * 0.02;
	roll += (x - lastX) * 0.02;

	lastX = x;
	lastY = y;

	camera.setEulerAngles ( yaw, pitch, roll );

	glutPostRedisplay ();
}
//////////////////////////////////////////
void    animate ()
{
	static	float	lastTime = 0.0;
	float			time     = 0.001f * glutGet ( GLUT_ELAPSED_TIME );

    angle   += 2 * (time - lastTime);
    lastTime = time;

	program.bind   ();
	program.setUniformFloat  ( "time",   time );
	program.setUniformVector ( "eyePos", camera.getPos () );
	program.unbind ();

	program2.bind   ();
	program2.setUniformFloat  ( "time",   time );
	program2.setUniformVector ( "eyePos", camera.getPos     () );
	program2.setUniformVector ( "up",     camera.getUpDir   () );
	program2.setUniformVector ( "right",  camera.getSideDir () );
	program2.unbind ();

    glutPostRedisplay ();
}

// Ask The User If He Wish To Enter GameMode Or Not
void ask_gamemode() {
//	int answer;
//	// Use Windows MessageBox To Ask The User For Game Or Windowed Mode
//	answer = MessageBox(NULL, "Do you want to enter game mode?", "Question",
//						MB_ICONQUESTION | MB_YESNO);
//	g_gamemode = (answer == IDYES);
//	// If Not Game Mode Selected, Use Windowed Mode (User Can Change That With F1)
//	g_fullscreen = false;
}

void ask(int argc, char** argv){
	QScopedPointer<QApplication> app(createApplication(argc, argv));
	QmlApplicationViewer viewer;
	viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
	viewer.setMainQmlFile(QLatin1String("../data/qml/settings.qml"));
	viewer.showExpanded();
	app->exec();
}

// Main Function For Bringing It All Together.
int main(int argc, char** argv) {
//	ask(argc, argv);
//	glutInit(&argc, argv);                           // GLUT Initializtion
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Display Mode (Rgb And Double Buffered)
//	if (g_gamemode) {
//		glutGameModeString("640x480:16");    // Select The 640x480 In 16bpp Mode
//		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
//			glutEnterGameMode();                     // Enter Full Screen
//		else
//			g_gamemode = false;    // Cannot Enter Game Mode, Switch To Windowed
//	}
//	if (!g_gamemode) {
//		glutInitWindowSize(500, 500); // Window Size If We Start In Windowed Mode
//		glutCreateWindow("NeHe's OpenGL Framework"); // Window Title
//	}
//	init();                                          // Our Initialization
//	glutDisplayFunc(render);                    // Register The Display Function
//	glutReshapeFunc(reshape);                    // Register The Reshape Handler
//	glutKeyboardFunc(keyboard);                 // Register The Keyboard Handler
//	glutSpecialFunc(special_keys);              // Register Special Keys Handler
//	glutMainLoop();                                  // Go To GLUT Main Loop
//	return 0;

//	Util::log("%s" , "TEST");
//	View * v = new View(&argc, argv);
//
//	// Open a window
//	v->createWindow("OpenGL grass rendering demo 5", 640, 480);

	glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize  ( 640, 480 );
								// create window
	glutCreateWindow ( "Example of HDR rendering" );

								// register handlers
	glutDisplayFunc       ( display    );
	glutReshapeFunc       ( reshape    );
	glutKeyboardFunc      ( key        );
	glutSpecialFunc       ( specialKey );
	glutPassiveMotionFunc ( mouseFunc  );
	glutIdleFunc          ( animate    );

    init();
    //initExtensions ();
    if (glewInit() != GLEW_OK) {
		printf("Error in glewInit\n");
		exit(0);
	}

    if ( !GLEW_ARB_shading_language_100 )
    {
        printf ( "GL_ARB_shading_language_100 NOT supported.\n" );

        return 1;
    }

    if ( !GLEW_ARB_shader_objects )
    {
        printf ( "GL_ARB_shader_objects NOT supported" );

        return 2;
    }


    decalMap = createTexture2D ( true, "grasslayer.png" );
	grassMap = createTexture2D ( true, "grassPack.png"  );
	noiseMap = createTexture2D ( true, "noise.png" );


    program.test();

	if ( !program.loadShaders ( "grass-3.vsh", "grass-3.fsh" ) )
	{
		printf ( "Error loading shaders:\n%s\n", program.getLog ().c_str () );

		return 3;
	}

	if ( !program2.loadShaders ( "grass-2.vsh", "grass-2.fsh" ) )
	{
		printf ( "Error loading shaders2:\n%s\n", program2.getLog ().c_str () );

		return 3;
	}

	printf ( "%d", grassMap );

    program.bind ();
    program.setTexture ( "grassMap",  0 );
	program.setTexture ( "noiseMap",  1 );
    program.unbind ();

    program2.bind ();
    program2.setTexture ( "grassMap",  0 );
	program2.setTexture ( "noiseMap",  1 );
    program2.unbind ();

	camera.setRightHanded ( false );

	initLandscape      ();
	initGrass          ( 0.4 );
	adjustCameraHeight ();

	printf ( "Starting\n" );

//    v->start();
	glutMainLoop ();

    return 0;
}
