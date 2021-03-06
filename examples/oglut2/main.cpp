/*                                        __         __
 *                          .-----.-----.|  |.--.--.|  |_
 * This file is part of the |  _  |  _  ||  ||  |  ||   _| series.
 *                          |_____|___  ||__||_____||____|
 *                                |_____| OpenGL and GLUT Tutorial
 *
 * OGLUT (OpenGL and GLUT Tutorial) is an introduction to using OpenGL
 * (http://www.opengl.org/) and GLUT (http://reality.sgi.com/opengl/glut3/).
 *
 * Copyright (C) 2001 Sebastian Schuberth (s.schuberth@tu-bs.de)
 * http://graphics.tu-bs.de/people/schuberth
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifdef WIN32
  #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

#ifndef M_PI
  #define M_PI 3.14159265358979323846264338327950288
#endif

static GLfloat angle=0.0f;

void initialize(void) {
  // Define light RGBA intensity parameters.
  GLfloat glfLightAmbient[]={0.1f,0.1f,0.1f,1.0f};
  GLfloat glfLightDiffuse[]={1.2f,1.2f,1.2f,1.0f};
  GLfloat glfLightSpecular[]={0.9f,0.9f,0.9f,1.0f};

  // Define a directional light source (infinite distance away from the objects in the scene).
  GLfloat glfLightPosition[]={0.0f,0.0f,2.0f,0.0f};

  // Set light source parameters.
  glLightfv(GL_LIGHT0,GL_AMBIENT,glfLightAmbient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,glfLightDiffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,glfLightSpecular);
  glLightfv(GL_LIGHT0,GL_POSITION,glfLightPosition);

  // Enable lighting in general.
  glEnable(GL_LIGHTING);
  // Enable the defined light source.
  glEnable(GL_LIGHT0);
  // Enable back face culling of polygons based upon their window coordinates.
  glEnable(GL_CULL_FACE);

  // Select flat shading.
  glShadeModel(GL_FLAT);
}

void display(void) {
  // Clear the buffers currently enabled for color writing.
  glClear(GL_COLOR_BUFFER_BIT);

  // Default matrix stack is the modelview matrix stack.
  glLoadIdentity();
  // Rotate around the x- and y-axis.
  GLfloat rad=angle*M_PI/180.0;
  glRotatef(angle,cos(rad),sin(rad),0.0f);

  // Render a solid cube.
  glutSolidCube(1.0);

  // Perform a buffer swap on the layer in use for the current window
  // (an implicit glFlush is done by glutSwapBuffers before it returns).
  glutSwapBuffers();
}

void idle(void) {
  angle+=1.0f;
  if (angle>=360.0f) angle=0.0f;

  // Mark the normal plane of current window as needing to be redisplayed.
  glutPostRedisplay();
}

void reshape(int width,int height) {
  // Apply subsequent matrix operations to the projection matrix stack.
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  // Adjust FOV, aspect ratio, near and far clipping plane.
  gluPerspective(30.0,(GLdouble)width/(GLdouble)height,1.0,10.0);
  // Move a little bit backwards so the scene becomes visible.
  glTranslatef(0.0f,0.0f,-5.0f);
  // Adjust the viewport to map the whole window size.
  glViewport(0,0,width,height);

  // Apply subsequent matrix operations to the modelview matrix stack.
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char **argv) {
  // Extract any command line options intended for GLUT.
  glutInit(&argc,argv);

  // Request an initial window size of 512 x 512 pixels.
  glutInitWindowSize(512,512);
  // Request an initial display mode with RGB color- and double-buffer.
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
  // Create a window with given title.
  glutCreateWindow("OpenGL GLUT Tutorial 02 - 3D Rotation and Directional Lighting");

  // Register a display callback function for window repaints.
  glutDisplayFunc(display);
  // Register an idle callback function for animations.
  glutIdleFunc(idle);
  // Register a reshape callback function for window resizes.
  glutReshapeFunc(reshape);

  // Initialize the rendering context.
  initialize();

  // Enter the GLUT main event processing loop.
  glutMainLoop();

  return 0;
}