/*
 * View.cpp
 *
 *  Created on: 27.12.2012
 *      Author: blacksmith
 */

#include "View.h"

View::View() :
		id(0), width(300), height(300), options(
				GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH) {
	// TODO Auto-generated constructor stub
}

View::View(int* pargc, char** argv) :
		id(0), width(300), height(300), options(
				GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH) {
	// TODO Auto-generated constructor stub
	glutInit(pargc, argv);
}

View::~View() {
	// TODO Auto-generated destructor stub
}

/**
 * set context options
 */
void View::setOptions(unsigned int opts) {
	this->options = opts;
}

/**
 * Create window
 */
void View::createWindow(const char * title, int width, int height) {
	this->width = width;
	this->height = height;
	// To see OpenGL drawing, take out the GLUT_DOUBLE request.
	glutInitDisplayMode(this->options);
	glutInitWindowSize(this->width, this->height);

	// Open a window
	this->id = glutCreateWindow(title);
}

/**
 * set display handler
 */
void View::onDraw(void (*callback)(void)) {
	// Register the callback function to do the drawing.
	glutDisplayFunc(callback);
}

/**
 * set idle handler
 */
void View::onIdle(void (*callback)(void)) {
	// If there's nothing to do, draw.
	glutIdleFunc(callback);
}

/**
 * set resize handler
 */
void View::onResize(void (*callback)(int, int)) {
	// It's a good idea to know when our window's resized.
	glutReshapeFunc(callback);
}

/**
 * set keyboard input handler
 */
void View::onKeyboard(void (*callback)(unsigned char, int, int)) {
	// And let's get some keyboard input.
	glutKeyboardFunc(callback);
}

/**
 * set mouse input handler
 */
void View::onMouse(void (* callback)( int, int, int, int )) {
	// And let's get some mouse input.
	glutMouseFunc(callback);
}


/**
 * set mouse input handler
 */
void View::onMouseMove(void (* callback)( int, int )) {
	// And let's get some mouse input.
	glutMotionFunc(callback);
	glutPassiveMotionFunc(callback);
}

/**
 * set special input handler
 */
void View::setSpecialHandler(void (*callback)(int, int, int)) {
	// And let's get some keyboard input.
	glutSpecialFunc(callback);
}

/**
 * start engine
 */
void View::start() {
	// Pass off control to OpenGL.
	// Above functions are called as appropriate.
	glutMainLoop();
}

int View::getWindowId(){
	return this->id;
}
