#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <assert.h>
#include "GL/glew.h"

#define ASSERT_GL_ERROR assert(glGetError()==GL_NO_ERROR);

#define PUSH_ORTHO_2D \
	/* Save the current matrix mode. */ \
	GLint _matrix_mode; \
	glGetIntegerv(GL_MATRIX_MODE,&_matrix_mode); \
	\
	/* Adjust the model view and projection matrices to simplify raster positioning. */ \
	glMatrixMode(GL_MODELVIEW); \
	glPushMatrix(); \
  glLoadIdentity(); \
	\
	glMatrixMode(GL_PROJECTION); \
  glPushMatrix(); \
  glLoadIdentity(); \
	GLint _viewport[4]; \
	glGetIntegerv(GL_VIEWPORT,_viewport); \
	glOrtho(_viewport[0],_viewport[2],_viewport[1],_viewport[3],-1,1);

#define POP_ORTHO_2D \
  /* Restore the model view and projection matrices. */ \
	glPopMatrix(); \
	\
	glMatrixMode(GL_MODELVIEW); \
	glPopMatrix(); \
	\
	/* Restore the matrix mode. */ \
	glMatrixMode(_matrix_mode);

void SetWindowRasterPos(int x,int y);

#endif // ENGINE_H_INCLUDED
