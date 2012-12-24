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

static GLfloat angle=0.0f;

void display(void) {
  // Clear the buffers currently enabled for color writing.
  glClear(GL_COLOR_BUFFER_BIT);

  // Default matrix stack is the modelview matrix stack.
  glLoadIdentity();
  // Rotate around the z-axis.
  glRotatef(angle,0.0f,0.0f,1.0f);

  // OpenGL uses a right-handed coordinate system like this:
  //
  //                         y
  //                         |
  //                         |
  //                         |
  //                         |
  //                         |
  //                         ---------- x
  //                        /
  //                       /
  //                      /
  //                     z

  // Treat each group of four vertices as an independent quadriliteral.
  glBegin(GL_QUADS);
    glVertex3f(+0.5f,+0.5f,0.0f);  // Upper right vertex.
    glVertex3f(-0.5f,+0.5f,0.0f);  // Upper left vertex.
    glVertex3f(-0.5f,-0.5f,0.0f);  // Lower left vertex.
    glVertex3f(+0.5f,-0.5f,0.0f);  // Lower right vertex.
  glEnd();

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

int main(int argc,char **argv) {
  // Extract any command line options intended for GLUT.
  glutInit(&argc,argv);

  // Request an initial window size of 512 x 512 pixels.
  glutInitWindowSize(512,512);
  // Request an initial display mode with RGB color- and double-buffer.
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
  // Create a window with given title.
  glutCreateWindow("OpenGL GLUT Tutorial 01 - 2D Rotation and Double-Buffering");

  // Register a display callback function for window repaints.
  glutDisplayFunc(display);
  // Register an idle callback function for animations.
  glutIdleFunc(idle);

  // Enter the GLUT main event processing loop.
  glutMainLoop();

  return 0;
}