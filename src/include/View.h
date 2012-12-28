/*
 * View.h
 *
 *  Created on: 27.12.2012
 *      Author: blacksmith
 */

#ifndef VIEW_H_
#define VIEW_H_

#include <GL/glew.h>   // GLEW extending.
#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
class View {
public:
	View();
	View(int* pargc, char** argv);
	virtual ~View();
	virtual void createWindow(const char * title, int width, int height);
	virtual void setOptions(unsigned int);
	virtual void onDraw(void (*callback)(void));
	virtual void onIdle(void (*callback)(void));
	virtual void onResize(void (*callback)(int, int));
	virtual void onKeyboard(void (*callback)(unsigned char, int, int));
	virtual void onMouse(void (*callback)(int, int, int, int));
	virtual void onMouseMove(void (*callback)(int, int));
	virtual void setSpecialHandler(void (*callback)(int, int, int));
	// Sets an orthographic projection. x: 0..width, y: 0..height
	static void BeginOrtho2D(float width, float height) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}

	// Sets an orthographic projection. x: x1..x2, y: y1..y2
	static void BeginOrtho2D(float x1, float y1, float x2, float y2) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(x1, x2, y1, y2);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}

	// Sets a perspective projection with a different field-of-view
	static void BeginPerspective(float fovy, float xratio) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(fovy, xratio, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}

	// Restore the previous projection
	static void EndProjection() {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	// Handles resizing of the window
	static void Resize(int x, int y) {
		// Prevent div by zero
		y = y ? y : 1;

		// Resize viewport
		glViewport(0, 0, x, y);

		// Recalc perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, static_cast<float>(x) / static_cast<float>(y),
				0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	virtual void start();
	int getWindowId();
private:
	int id;
	unsigned int width;
	unsigned int height;
	unsigned int options;
};

#endif /* VIEW_H_ */
