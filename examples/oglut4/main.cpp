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
#include <stdio.h>
#include <malloc.h>

#ifndef M_PI
  #define M_PI 3.14159265358979323846264338327950288
#endif

static GLfloat angle=0.0f,s=0.0f,c=0.0f;
static bool pause=false;

static GLuint texture_list[6];
static const char *texture_file[6]={
  "../textures/batgirl.raw",
  "../textures/batlogo.raw",
  "../textures/batman.raw",
  "../textures/freeze.raw",
  "../textures/poison.raw",
  "../textures/robin.raw"
};

static char title[44+13+1]="OpenGL GLUT Tutorial 04 - 2D Texture-Mapping";

void load(const char *filename) {
  FILE *handle;

  if (!(handle=fopen(filename,"rb"))) {
    fputs("[OpenGL GLUT Tutorial 04] Error: Unable to open texture files.",stderr);
    exit(-1);
  }

  unsigned char *image=(unsigned char *)malloc(128*128*3);

  if (fread(image,128*128*3,1,handle)!=1) {
    fputs("[OpenGL GLUT Tutorial 04] Error: Unable to read texture files.",stderr);
    free(image);
    exit(-1);
  }
  fclose(handle);

  // Specify a two-dimensional texture image with parameters:
  // level-of-detail, number of color components, width and height of the
  // texture image, width of the border, format and data type of the pixel data.
  glTexImage2D(GL_TEXTURE_2D,0,3,128,128,0,GL_RGB,GL_UNSIGNED_BYTE,image);

  free(image);
}

void initialize(void) {
  // Enable back face culling of polygons based upon their window coordinates.
  glEnable(GL_CULL_FACE);
  // Enable two-dimensional texturing.
  glEnable(GL_TEXTURE_2D);

  // Generate 6 texture names (one for each face of the cube).
  glGenTextures(6,texture_list);

  for (int i=0;i<6;i++) {
    // Create a named texture.
    glBindTexture(GL_TEXTURE_2D,texture_list[i]);

    // The texture minifying function is used whenever the pixel being textured
    // maps to an area greater than one texture element (this means the texture
    // must be shrunk in size). GL_LINEAR returns the weighted average of the
    // four texture elements that are closest to the center of the pixel being textured.
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    // The texture magnification function is used when the pixel being textured
    // maps to an area less than or equal to one texture element (this means the
    // texture must be magnified). GL_LINEAR returns the weighted average of the
    // four texture elements that are closest to the center of the pixel being textured.
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // GL_CLAMP causes texture coordinates to be clamped to the range [0,1] and is
    // useful for preventing wrapping artifacts when mapping a single image onto
    // an object.
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

    load(texture_file[i]);
  }
}

void display(void) {
  static int time_base=0,frame=0;

  frame++;
  // Get the number of milliseconds since "glutInit" has been called.
  int time=glutGet(GLUT_ELAPSED_TIME);

  if (time-time_base>1000) {
    sprintf(&title[44]," (%3.2f FPS)",frame*1000.0f/(time-time_base));
    glutSetWindowTitle(title);
    time_base=time;
    frame=0;
  }

  // Clear the color-buffer.
  glClear(GL_COLOR_BUFFER_BIT);

  // Default matrix stack is the modelview matrix stack.
  glLoadIdentity();
  // Rotate around the x- and y-axis.
  glRotatef(angle,c,s,0.0f);

  // Draw the textured cube. Take note that the texture vertex must come
  // BEFORE the vertex that it will be assigned to.

  // Texture coordinate orientation (x,y):
  // (0,0)-----------(1,0)
  //   |               |
  //   |     Image     |
  //   |               |
  // (0,1)-----------(1,1)

  for (int i=0;i<4;i++) {
    // Push the current matrix stack (there is a stack of matrices for each of the matrix modes).
    glPushMatrix();

    // Rotate around the y-axis to create the front, back, left and right faces.
    glRotatef(i*90.0f,0.0f,1.0f,0.0f);
    // Move along the z-axis (towards the camera).
    glTranslatef(0.0f,0.0f,1.0f);

    // Use a named texture.
    glBindTexture(GL_TEXTURE_2D,texture_list[i]);
    // Define the front face.
    glBegin(GL_QUADS);
      glTexCoord2f(1.0f,0.0f);
      glVertex3f(+1.0f,+1.0f,0.0f);  // Upper right vertex.
      glTexCoord2f(0.0f,0.0f);
      glVertex3f(-1.0f,+1.0f,0.0f);  // Upper left vertex.
      glTexCoord2f(0.0f,1.0f);
      glVertex3f(-1.0f,-1.0f,0.0f);  // Lower left vertex.
      glTexCoord2f(1.0f,1.0f);
      glVertex3f(+1.0f,-1.0f,0.0f);  // Lower right vertex.
    glEnd();

    // Pop the current matrix stack.
    glPopMatrix();
  }

  // Push the current matrix stack (there is a stack of matrices for each of the matrix modes).
  glPushMatrix();
  // Rotate around the x-axis to create the bottom face.
  glRotatef(90.0f,1.0f,0.0f,0.0f);
  // Move along the z-axis (towards the camera).
  glTranslatef(0.0f,0.0f,1.0f);
  // Use a named texture.
  glBindTexture(GL_TEXTURE_2D,texture_list[4]);
  // Define the front face.
  glBegin(GL_QUADS);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(+1.0f,+1.0f,0.0f);  // Upper right vertex.
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-1.0f,+1.0f,0.0f);  // Upper left vertex.
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-1.0f,-1.0f,0.0f);  // Lower left vertex.
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(+1.0f,-1.0f,0.0f);  // Lower right vertex.
  glEnd();
  // Pop the current matrix stack.
  glPopMatrix();

  // Rotate around the x-axis to create the top face.
  glRotatef(-90.0f,1.0f,0.0f,0.0f);
  // Move along the z-axis (towards the camera).
  glTranslatef(0.0f,0.0f,1.0f);
  // Use a named texture.
  glBindTexture(GL_TEXTURE_2D,texture_list[5]);
  // Define the front face.
  glBegin(GL_QUADS);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(+1.0f,+1.0f,0.0f);  // Upper right vertex.
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-1.0f,+1.0f,0.0f);  // Upper left vertex.
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-1.0f,-1.0f,0.0f);  // Lower left vertex.
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(+1.0f,-1.0f,0.0f);  // Lower right vertex.
  glEnd();

  // Perform a buffer swap on the layer in use for the current window
  // (an implicit glFlush is done by glutSwapBuffers before it returns).
  glutSwapBuffers();
}

void idle(void) {
  if (pause) return;

  angle+=0.3f;
  if (angle>=360.0f) angle=0.0f;

  GLfloat rad=angle*M_PI/180.0;
  s=sin(rad);
  c=cos(rad);

  // Mark the normal plane of current window as needing to be redisplayed.
  glutPostRedisplay();
}

void reshape(int width,int height) {
  // Apply subsequent matrix operations to the projection matrix stack.
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  // Adjust FOV, aspect ratio, near and far clipping plane.
  gluPerspective(45.0,(GLdouble)width/(GLdouble)height,1.0,10.0);
  // Move a little bit backwards so the scene becomes visible.
  glTranslatef(0.0f,0.0f,-5.0f);
  // Adjust the viewport to map the whole window size.
  glViewport(0,0,width,height);

  // Apply subsequent matrix operations to the modelview matrix stack.
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key,int x,int y) {
  // Press 'p' to toggle pause state.
  if (key=='p') pause=!pause;

  // Press 'q' to quit. Note that this way the texture list is deleted, which is
  // not done when using the mouse to close the window.
  if (key=='q') {
    glDeleteTextures(7,texture_list);
    exit(0);
  }
}

int main(int argc,char **argv) {
  // Extract any command line options intended for GLUT.
  glutInit(&argc,argv);

  // Request an initial window size of 512 x 512 pixels.
  glutInitWindowSize(512,512);
  // Request an initial display mode with RGB color- and double-buffer.
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
  // Create a window with given title.
  glutCreateWindow(title);

  // Register a display callback function for window repaints.
  glutDisplayFunc(display);
  // Register an idle callback function for animations.
  glutIdleFunc(idle);
  // Regsiter a reshape callback function for window resizes.
  glutReshapeFunc(reshape);
  // Register a keyboard callback function to receive key presses.
  glutKeyboardFunc(keyboard);

  // Initialize the rendering context.
  initialize();

  // Enter the GLUT main event processing loop.
  glutMainLoop();

  return 0;
}
