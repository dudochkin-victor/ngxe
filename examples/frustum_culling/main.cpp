#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "frustum.h"
#include "geometry.h"

// Disable the console window in the release build.
#if defined(_MSC_VER) && defined(NDEBUG)
  #pragma comment(linker,"/entry:mainCRTStartup")
  #pragma comment(linker,"/subsystem:windows")
#endif

#define GLUT_KEY_ENTER  13
#define GLUT_KEY_ESCAPE 27

static char *wnd_title="Frustum Culling";

static double angle=0;
static float azimuth[24];
static float elevation[24];

static Teapot teapot;
static FrustumCulling fc;

// User interaction variables.
static bool wnd_full_screen=false,render_pause=false,render_aabb=true;
static int render_culling=0;
static float fps,fps_min=1024,fps_max=0;

void initialize() {
  #ifdef _DEBUG
	puts("Keys\n----");
	puts("c\tCycle through no frustum culling, software and hardware frustum culling");
	puts("b\tToggle displaying of bounding boxes");
	puts("p\tToggle rendering pause\n");
	puts("State\n-----");
	printf("frustum culling mode : %d\n",render_culling);
	printf("show bounding boxes  : %d\n",render_aabb);
	printf("pause rendering      : %d\n\n",render_pause);
  #endif

	//srand((unsigned int)time(NULL));
	for (int i=23;i>=0;--i) {
		azimuth[i]=rand()*360.0f/RAND_MAX;
    elevation[i]=rand()*40.0f/RAND_MAX-20.0f;
	}
}

void render(){
	teapot.render();
 	if (render_aabb) teapot.renderAABB();
}

void display() {
  static int time_start=0,frame=0;
	static char wnd_title_fps[2048];

  ++frame;
  // Get the number of milliseconds since "glutInit()" has been called.
  int time_elapsed=glutGet(GLUT_ELAPSED_TIME)-time_start;

  // Update the FPS counter every 500 milliseconds.
	if (time_elapsed>500) {
		fps=frame*1000.0f/time_elapsed;
		if (fps<fps_min) fps_min=fps;
		if (fps>fps_max) fps_max=fps;
    sprintf(wnd_title_fps,"%s @ %.2f (min. %.2f, max. %.2f) FPS",wnd_title,fps,fps_min,fps_max);
    time_start+=time_elapsed;
    frame=0;
    glutSetWindowTitle(wnd_title_fps);
  }

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	for (int i=23;i>=0;--i) {
		glLoadIdentity();
		gluLookAt(-1,0,0,0,0,0,0,1,0);

		glRotatef(angle+azimuth[i],0,1,0);
		glRotatef(elevation[i],0,0,1);
		glTranslatef(10,0,0);

		switch (render_culling) {
			case 0: {
				render();
				break;
			}
			case 1: {
				if (fc.isVisible(teapot)) render();
				break;
			}
			case 2: {
				static GLuint buffer[1024];

				glSelectBuffer(1024,buffer);
				glRenderMode(GL_SELECT);
				teapot.renderAABB();
				if (glRenderMode(GL_RENDER)>0) render();
			}
		}
	}

  glutSwapBuffers();
}

void idle() {
  if (render_pause) return;
	
	angle+=0.9;

	glutPostRedisplay();
}

void reshape(int width,int height) {
  // Avoid division by zero.
	if (height<=0) height=1;

	// Set the viewport rectangle.
  glViewport(0,0,(GLsizei)width,(GLsizei)height);

  // Specify the viewing frustum.
	GLint matrix_mode;
	glGetIntegerv(GL_MATRIX_MODE,&matrix_mode);

	glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,(GLdouble)width/height,0.01,100.0);

  glMatrixMode(matrix_mode);

	// Calculate the view frustum planes.
	fc.prepare();
}

void keyboard(unsigned char key,int x,int y) {
	// Toggle full screen display.
	if ((key==GLUT_KEY_ENTER)&&(glutGetModifiers()==GLUT_ACTIVE_ALT)) {
		static int wnd_x,wnd_y,wnd_width,wnd_height;

		if (wnd_full_screen^=true) {
			wnd_x=glutGet(GLUT_WINDOW_X);
			wnd_y=glutGet(GLUT_WINDOW_Y);
			wnd_width=glutGet(GLUT_WINDOW_WIDTH);
			wnd_height=glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();
		} else {
			glutReshapeWindow(wnd_width,wnd_height);
			glutPositionWindow(wnd_x,wnd_y);
		}
	}

	// Exit the application when "Esc" is pressed.
	if (key==GLUT_KEY_ESCAPE) exit(EXIT_SUCCESS);

	// Toggle frustum culling.
	if (key=='c') {
	  render_culling=(render_culling+1)%3;
		#ifdef _DEBUG
		printf("frustum culling mode : %d\n",render_culling);
    #endif
		
		// Reset minimum and maximum frames per second values.
		fps_min=1024;
		fps_max=0;

		glutPostRedisplay();
	}

	// Toggle display of bounding boxes.
	if (key=='b') {
	  render_aabb^=true;
		#ifdef _DEBUG
		printf("show bounding boxes  : %d\n",render_aabb);
    #endif
		glutPostRedisplay();
	}

	// Toggle rendering pause.
	if (key=='p') {
	  render_pause^=true;
		#ifdef _DEBUG
		printf("pause rendering      : %d\n",render_pause);
    #endif
		glutPostRedisplay();
	}
}

void special(int key,int x,int y) {
	// Exit the application when "Alt-F4" is pressed.
	if ((key==GLUT_KEY_F4)&&(glutGetModifiers()==GLUT_ACTIVE_ALT)) exit(EXIT_SUCCESS);
}

int main(int argc,char **argv) {
	initialize();

	// Initialize the GLUT library.
  glutInit(&argc,argv);

  // Set the initial window size and position.
  int width=glutGet(GLUT_SCREEN_WIDTH)>>1;
  int height=glutGet(GLUT_SCREEN_HEIGHT)>>1;

	glutInitWindowSize(width,height);
  glutInitWindowPosition(width>>1,height>>1);

  // Set the initial display mode.
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
  // Create a top-level window.
  glutCreateWindow(wnd_title);

	// Register window callbacks.
  glutDisplayFunc(display);
	glutIdleFunc(idle);
  glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	// Initialize OpenGL states and any necessary data.
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Enter the GLUT main event processing loop.
	glutMainLoop();

  return 0;
}