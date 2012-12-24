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
#include <malloc.h>
#include <stdio.h>
#include "ijl.h"

#define PICTURE_WIDTH  640
#define PICTURE_HEIGHT 480

static unsigned char *picture,*picture1,*picture2,*stencil;
static GLint stencil_reference=0,transition_step=1,transition_count=0;

void load(char *filename,unsigned char **image,unsigned int channels=3) {
  // Allocate the IJL JPEG_CORE_PROPERTIES structure.
  JPEG_CORE_PROPERTIES jcprops;

  // Initialize the Intel JPEG Library.
  if (ijlInit(&jcprops)!=IJL_OK) {
    MessageBox(NULL,"Unable to initialize the Intel JPEG Library.","Error",MB_ICONEXCLAMATION);
    exit(-1);
  }

  jcprops.JPGFile=filename;
  // Get information on the JPEG image (i.e. width, height and channels).
  if (ijlRead(&jcprops,IJL_JFILE_READPARAMS)!=IJL_OK) {
    MessageBox(NULL,"Unable to read JPEG image information.","Error",MB_ICONEXCLAMATION);
    exit(-1);
  }

  // Store the bytes per image row, as we need it several times.
  unsigned int bpr=jcprops.JPGWidth*channels;
  // Allocate some memory for the decoded image.
  *image=(unsigned char *)malloc(bpr*jcprops.JPGHeight);

  // Set up the info on the desired DIB properties.
  jcprops.DIBWidth=jcprops.JPGWidth;
  jcprops.DIBHeight=jcprops.JPGHeight;
  jcprops.DIBChannels=channels;
  jcprops.DIBColor=(channels==3)?IJL_RGB:IJL_G;
  jcprops.DIBPadBytes=0;
  jcprops.DIBBytes=*image;

  // Now get the actual JPEG image data into the pixel buffer.
  if (ijlRead(&jcprops,IJL_JFILE_READWHOLEIMAGE)!=IJL_OK) {
    MessageBox(NULL,"Unable to read JPEG image data.","Error",MB_ICONEXCLAMATION);
    exit(-1);
  }

  // Clean up the Intel JPEG Library.
  ijlFree(&jcprops);
}

void initialize(void) {
	// Since OpenGL draws bitmaps "upside-down" we need to flip the image ...
	glPixelZoom(1.0f,-1.0f);
	// ... and adjust the raster position accordingly.
  glRasterPos2f(-1.0f,1.0f);

  // Load the background pictures into memory.
	load("images/picture1.jpg",&picture1);
  load("images/picture2.jpg",&picture2);
  // Initialize the current picture pointer to "picture1".
  picture=picture1;
  
	// Enable stencil-buffer testing.
  glEnable(GL_STENCIL_TEST);
}

void display(void) {
  if (!stencil) {
		// Load the stencil picture into memory.
		load("images/stencil.jpg",&stencil,1);
		// Transfer the stencil mask to the OpenGL stencil-buffer.
		glDrawPixels(PICTURE_WIDTH,PICTURE_HEIGHT,GL_STENCIL_INDEX,GL_UNSIGNED_BYTE,stencil);
  }

	// Clear the color and depth buffers.
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw a pixel if "stencil_reference" is less or equal to the stencil-buffer content.
  // In this case, a refernce value of 0 means "all pixels visible" and a value of 256
  // means "all pixels are invisible" (i.e. the screen is black).
  glStencilFunc(GL_LEQUAL,stencil_reference,0x1FF);
  // Show the picture on screen.
	glDrawPixels(PICTURE_WIDTH,PICTURE_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,picture);

  // Transition mode one fades a picture completely to black before fading the other
  // one in. Mode two mixes parts of both pictures together, going from "much of
	// picture one, less of picture two" to the opposite.
  if (transition_count<2) {
    // If the screen just faded to black, exchange the pictures.
    if (stencil_reference==256) picture=(picture==picture1)?picture2:picture1;
  } else {
    glStencilFunc(GL_GREATER,stencil_reference,0x1FF);
    // Show the picture on screen.
    glDrawPixels(PICTURE_WIDTH,PICTURE_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,picture2);
  }

  // Perform a buffer swap on the layer in use for the current window
  // (an implicit glFlush is done by glutSwapBuffers before it returns).
  glutSwapBuffers();
}

void idle(void) {
  // Increase / decrease the stencil reference value.
  stencil_reference+=transition_step;

  // Shortcut for "if ((stencil_reference==0)||(stencil_reference==256)) ...".
  if ((stencil_reference&0xFF)==0) {
    // The "transition_count" is used to switch between transition modes one and two.
    if (stencil_reference==0) transition_count=(transition_count+1)%3;
    transition_step=-transition_step;
  }

  // Mark the normal plane of current window as needing to be redisplayed.
  glutPostRedisplay();
}

void reshape(int width,int height) {
  // Do not allow the window to be resized.
	glutReshapeWindow(PICTURE_WIDTH,PICTURE_HEIGHT);
}

int oglutCreateWindow(char *name,int width,int height,unsigned int mode) {
  // Request an initial window size.
  glutInitWindowSize(width,height);
  // Set the initial display mode.
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|mode);
  // Create a top-level window.
  return glutCreateWindow(name);
}

void oglutFreeMemory(void) {
	#ifdef _DEBUG
	puts("Freeing used memory ...");
	#endif
	// Free the memory used by the pictures.
  free(picture1);
  free(picture2);
  // Free the memory used by the stencil mask.
  free(stencil);
}

int main(int argc,char **argv) {
  // Extract any command line options intended for GLUT.
  glutInit(&argc,argv);
  // Create a top-level window using the given parameters.
  oglutCreateWindow("OpenGL GLUT Tutorial 05 - Stencil-Buffer Effects",PICTURE_WIDTH,PICTURE_HEIGHT,GLUT_STENCIL);
  // Register the exit callback.
  atexit(oglutFreeMemory);

  // Register a display callback function for window repaints.
  glutDisplayFunc(display);
  // Register an idle callback function for animations.
  glutIdleFunc(idle);
  // Regsiter a reshape callback function for window resizes.
  glutReshapeFunc(reshape);

  // Initialize the rendering context.
  initialize();

	// Enter the GLUT main event processing loop.
  glutMainLoop();

  return 0;
}