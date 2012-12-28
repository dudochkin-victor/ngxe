/*
 * View.h
 *
 *  Created on: 27.12.2012
 *      Author: blacksmith
 */

#ifndef VIEW_H_
#define VIEW_H_

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
	virtual void onDraw( void (* callback) (void));
	virtual void onIdle(void (* callback) (void));
	virtual void onResize(void (* callback) (int, int));
	virtual void onKeyboard(void (* callback) (unsigned char, int, int));
	virtual void onMouse(void (* callback)( int, int, int, int ));
	virtual void onMouseMove(void (* callback)( int, int ));
	virtual void setSpecialHandler(void (* callback) (int, int, int));
	virtual void start();
	int getWindowId();
private:
	int id;
	unsigned int width;
	unsigned int height;
	unsigned int options;
};

#endif /* VIEW_H_ */
