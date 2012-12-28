#include "buffer.h"
#include <float.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

// Disable the console window in the release build.
#if defined(_MSC_VER) && defined(NDEBUG)
#pragma comment(linker,"/entry:mainCRTStartup")
#pragma comment(linker,"/subsystem:windows")
#endif

#define GLUT_KEY_ENTER  13
#define GLUT_KEY_ESCAPE 27

// Window title variables.
static const char *wnd_title = "Buffer Benchmark";
static char wnd_title_size[2048], wnd_title_fps[2048];

// User interaction variables.
static bool wnd_full_screen = false, render_pause = false;
static bool show_buffer = false, trans_op = true;

// Animation variables.
static float angle_x = 0, angle_y = 0, angle_z = 0;

static Buffer *buffer;
static GLenum format = 0;

void construct() {
#ifdef _DEBUG
	puts("\nWindow keys\n-----------");
	puts("Alt+Enter     : Toggle fullscreen");
	puts("Alt+F4 or Esc : Exit");
	puts("\nAnimation keys\n--------------");
	puts("p : Toggle rendering pause");
	puts("n : Read from nowhere");
	puts("c : Read from color buffer");
	puts("d : Read from depth buffer");
	puts("s : Read from stencil buffer");
	puts("b : Toggle buffer display");
	puts("t : Toggle pixel transfer operations");
#endif

	buffer = new Buffer();
}

void destroy() {
#ifdef _DEBUG
	puts("\nFreeing used memory.");
#endif

	delete buffer;
}

void render() {
	GLfloat red_scale, green_scale, blue_scale;
	GLfloat depth_scale, depth_bias;
	GLfloat index_offset;

	switch (format) {
	case GL_LUMINANCE: {
		if (!trans_op)
			break;
		glGetFloatv(GL_RED_SCALE, &red_scale);
		glPixelTransferf(GL_RED_SCALE, FLT_MAX);
		glGetFloatv(GL_GREEN_SCALE, &green_scale);
		glPixelTransferf(GL_GREEN_SCALE, FLT_MAX);
		glGetFloatv(GL_BLUE_SCALE, &blue_scale);
		glPixelTransferf(GL_BLUE_SCALE, FLT_MAX);
		break;
	}
	case GL_DEPTH_COMPONENT: {
		if (!trans_op)
			break;
		// A depth buffer value z ranges from 0.0 (near clipping plane) to 1.0 (far
		// clipping plane). Calculating (1-z)*r = r-z*r = z*(-r)+r makes the depth
		// values range from r to 0.0.
		glGetFloatv(GL_DEPTH_SCALE, &depth_scale);
		glPixelTransferf(GL_DEPTH_SCALE, -FLT_MAX);
		glGetFloatv(GL_DEPTH_BIAS, &depth_bias);
		glPixelTransferf(GL_DEPTH_BIAS, FLT_MAX);
		break;
	}
	case GL_STENCIL_INDEX: {
		glPushAttrib(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);

		// Fill the stencil buffer with 1-bits.
		glClearStencil(1);
		glClear(GL_STENCIL_BUFFER_BIT);
		// Write a 0-bit to the stencil buffer for every rendered pixel.
		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		if (!trans_op)
			break;
		// Invert bits during the pixel transfer.
		glGetFloatv(GL_INDEX_OFFSET, &index_offset);
		glPixelTransferi(GL_INDEX_OFFSET, 255);
	}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

	glRotatef(angle_x, 1, 0, 0);
	glRotatef(angle_y, 0, 1, 0);
	glRotatef(angle_z, 0, 0, 1);

	glFrontFace(GL_CW);
	glutSolidTeapot(1);
	glFrontFace(GL_CCW);

	if (format)
		buffer->read(format);

	switch (format) {
	case GL_LUMINANCE: {
		if (!trans_op)
			break;
		glPixelTransferf(GL_RED_SCALE, red_scale);
		glPixelTransferf(GL_GREEN_SCALE, green_scale);
		glPixelTransferf(GL_BLUE_SCALE, blue_scale);
		break;
	}
	case GL_DEPTH_COMPONENT: {
		if (!trans_op)
			break;
		glPixelTransferf(GL_DEPTH_SCALE, depth_scale);
		glPixelTransferf(GL_DEPTH_BIAS, depth_bias);
		break;
	}
	case GL_STENCIL_INDEX: {
		glPopAttrib();

		if (!trans_op)
			break;
		glPixelTransferf(GL_INDEX_OFFSET, index_offset);
	}
	}

	if (show_buffer)
		buffer->draw(256, 256);
}

void display() {
	static int time_start = 0, frame = 0;

	++frame;
	// Get the number of milliseconds since "glutInit()" has been called.
	int time_elapsed = glutGet(GLUT_ELAPSED_TIME) - time_start;

	// Update the FPS counter every 500 milliseconds.
	if (time_elapsed > 500) {
		sprintf(wnd_title_fps, "%s @ %.2f FPS", wnd_title_size,
				frame * 1000.0f / time_elapsed);
		time_start += time_elapsed;
		frame = 0;
		glutSetWindowTitle(wnd_title_fps);
	}

	render();

	glutSwapBuffers();
}

void idle() {
	if (render_pause)
		return;

	angle_x = angle_x + 0.2;
	angle_y = angle_y + 0.3;
	angle_z = angle_z + 0.1;

	glutPostRedisplay();
}

void reshape(int width, int height) {
	// Set the viewport rectangle.
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	buffer->setReadDims(0, 0, width, height);

	// Print the window size to the title bar.
	sprintf(wnd_title_size, "%s (%d x %d)", wnd_title, width, height);

	// Avoid division by zero.
	if (height <= 0)
		return;

	// Specify the viewing frustum.
	GLint matrix_mode;
	glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble) width / height, 0.01, 100.0);

	glMatrixMode(matrix_mode);
}

void keyboard(unsigned char key, int x, int y) {
	// Toggle full screen display.
	if ((key == GLUT_KEY_ENTER) && (glutGetModifiers() == GLUT_ACTIVE_ALT)) {
		static int wnd_x, wnd_y, wnd_width, wnd_height;

		if (wnd_full_screen ^= true) {
			wnd_x = glutGet(GLUT_WINDOW_X);
			wnd_y = glutGet(GLUT_WINDOW_Y);
			wnd_width = glutGet(GLUT_WINDOW_WIDTH);
			wnd_height = glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();
		} else {
			glutReshapeWindow(wnd_width, wnd_height);
			glutPositionWindow(wnd_x, wnd_y);
		}
	}

	// Exit the application when "Esc" is pressed.
	if (key == GLUT_KEY_ESCAPE)
		exit(EXIT_SUCCESS);

	// Toggle rendering pause.
	if (key == 'p') {
		render_pause ^= true;
		glutPostRedisplay();
	}

	// Set the buffer to read the pixels from.
	if (key == 'n') {
		format = 0;
		show_buffer = false;
		glutPostRedisplay();
	}

	if (key == 'c') {
		format = GL_LUMINANCE;
		show_buffer = true;
		glutPostRedisplay();
	}

	if (key == 'd') {
		format = GL_DEPTH_COMPONENT;
		show_buffer = true;
		glutPostRedisplay();
	}

	if (key == 's') {
		format = GL_STENCIL_INDEX;
		show_buffer = true;
		glutPostRedisplay();
	}

	// Toggle buffer display.
	if (key == 'b') {
		show_buffer ^= true;
		glutPostRedisplay();
	}

	// Toggle pixel transfer operations.
	if (key == 't') {
		trans_op ^= true;
		glutPostRedisplay();
	}
}

void special(int key, int x, int y) {
	// Exit the application when "Alt-F4" is pressed.
	if ((key == GLUT_KEY_F4) && (glutGetModifiers() == GLUT_ACTIVE_ALT))
		exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	// Initialize the GLUT library.
	glutInit(&argc, argv);

	// Set the initial window size and position.
	int width = glutGet(GLUT_SCREEN_WIDTH) >> 1;
	int height = glutGet(GLUT_SCREEN_HEIGHT) >> 1;

	glutInitWindowSize(width, height);
	glutInitWindowPosition(width >> 1, height >> 1);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
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

	// Initialize OpenGL extensions.
	glewInit();

	construct();
	atexit(destroy);

	// Enter the GLUT main event processing loop.
	glutMainLoop();

	return 0;
}
