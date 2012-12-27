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
	View(int width, int height);
	virtual ~View();
	virtual void createWindow(const char * title);
	virtual void setOptions(unsigned int);
	virtual void setDisplayHandler( void (* callback) (void));
	virtual void setIdleHandler(void (* callback) (void));
	virtual void setReshapeHandler(void (* callback) (int, int));
	virtual void setKeyboardHandler(void (* callback) (unsigned char, int, int));
	virtual void setSpecialHandler(void (* callback) (int, int, int));
	virtual void start();
private:
	int id;
	unsigned int width;
	unsigned int height;
	unsigned int options;
};

#endif /* VIEW_H_ */
