/*
Copyright (C) 2005 Heinrich du Toit

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

The license can also be obtained electronically at www.fsf.org or www.freesoftware.org/licenses

I can be electronically reached at the following email addresses:
13806335@sun.ac.za (will work till end of 2005)
nlhg@eject.co.za

For header files (which is included in other files at time of compilation) only the following notice is inserted:

This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit

 */
/*
 *
 *  By Heinrich du Toit
 *  
 * Vertex arrays example code
 * Tutorial 7
 *
 *
 */


#include <iostream>
using namespace std;

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <video.h>
#include <event.h>
#include <light.h>


#define TRUE 1
#define FALSE 0


#define NRVERTICES 16
GLfloat box[] = {1,1,-1, //side1
		 -1,1,-1,
		 -1,1,1,
		 1,1,1,
		 -1,-1,-1, //side2
		 1,-1,-1,
		 1,-1,1,
		 -1,-1,1,
		 -1,1,-1, //side3
		 -1,-1,-1,
		 -1,-1,1,
		 -1,1,1,
		 1,1,-1, //side4
		 1,-1,-1,
		 1,-1,1,
		 1,1,1};

GLfloat colors[] ={1,1,1,
		   1,1,1,
		   1,1,1,
		   1,1,1,
		   1,0,0,
		   1,0,0,
		   1,0,0,
		   1,0,0,
		   0,1,0,
		   0,1,0,
		   0,1,0,
		   0,1,0,
		   0,0,1,
		   0,0,1,
		   0,0,1,
		   0,0,1};

//use buffer objects to put vertex data into video card memory?
#define USEBUFFEROBJECT

#ifdef USEBUFFEROBJECT
//buffer object only became part of opengl 1.5

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * GLBINDBUFFERARBPROC)(GLenum target,GLuint buffer);
typedef void (APIENTRY * GLDELETEBUFFERSARBPROC)(GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * GLGENBUFFERSARBPROC)(GLsizei n, GLuint *buffers);
typedef void (APIENTRY * GLBUFFERDATAARBPROC)(GLenum target,int size, const GLvoid *data,GLenum usage);

GLGENBUFFERSARBPROC glGenBuffersARB = NULL;
GLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
GLBINDBUFFERARBPROC glBindBufferARB = NULL;
GLBUFFERDATAARBPROC glBufferDataARB = NULL;

GLuint buffers[2];
#endif


void Setup()
{
  //enable array usage:
  glEnable(GL_VERTEX_ARRAY);
  glEnable(GL_COLOR_ARRAY);
  /*
   * The value to use with glEnable with other arrays:
   *  GL_VERTEX_ARRAY
   *  GL_NORMAL_ARRAY
   *  GL_COLOR_ARRAY
   *  GL_SECONDARY_COLOR_ARRAY
   *  GL_INDEX_ARRAY
   *  GL_FOG_COORDINATE_ARRAY
   *  GL_TEXTURE_COORD_ARRAY
   *  GL_EDGE_FLAG_ARRAY
   */

#ifdef USEBUFFEROBJECT
  //first get ARB Extension functions
  glGenBuffersARB = (GLGENBUFFERSARBPROC)SDL_GL_GetProcAddress("glGenBuffersARB");
  glDeleteBuffersARB = (GLDELETEBUFFERSARBPROC)SDL_GL_GetProcAddress("glDeleteBuffersARB");
  glBindBufferARB = (GLBINDBUFFERARBPROC)SDL_GL_GetProcAddress("glBindBufferARB");
  glBufferDataARB = (GLBUFFERDATAARBPROC)SDL_GL_GetProcAddress("glBufferDataARB");
  
  if ((!glGenBuffersARB)||(!glDeleteBuffersARB)||(!glBindBufferARB)||(!glBufferDataARB)){
    cout << "Can't get GL_ARB_vertex_buffer_object functions" << endl;
    exit(-1);
  }
 
  //create a buffer
  glGenBuffersARB(2,buffers);

  //first the vertices:

  //activate/bind buffer
  glBindBufferARB(GL_ARRAY_BUFFER_ARB,buffers[0]);
  //load data into buffer
  glBufferDataARB(GL_ARRAY_BUFFER_ARB,3*sizeof(GLfloat)*NRVERTICES,box,GL_STATIC_DRAW_ARB);

  //now the colors:

  //activate/bind buffer
  glBindBufferARB(GL_ARRAY_BUFFER_ARB,buffers[1]);
  //load data into buffer
  glBufferDataARB(GL_ARRAY_BUFFER_ARB,3*sizeof(GLfloat)*NRVERTICES,colors,GL_STATIC_DRAW_ARB);


  //all that remains is to tell opengl that the arrays is now inside these buffer objects:
  //vertices:
  glBindBufferARB(GL_ARRAY_BUFFER_ARB,buffers[0]);
  glVertexPointer(3,GL_FLOAT,0,NULL);
  //colors:
  glBindBufferARB(GL_ARRAY_BUFFER_ARB,buffers[1]);
  glColorPointer(3,GL_FLOAT,0,NULL);

  /*
   * If you call a gl*Pointer function with data pointer=NULL
   * opengl binds to the current active GL_ARRAY_BUFFER object
   * Quite easy! (see below for more details)
   */

#else
  //no buffer object to use:
  
  //tell opengl about the arrays:
  glVertexPointer(3,GL_FLOAT,0,box);
  glColorPointer(3,GL_FLOAT,0,colors);

#endif
  /* 
   * Commands to tell opengl the location of arrays:
   *  glVertexPointer
   *  glNormalPointer
   *  glColorPointer 
   *  glSecondaryColorPointer
   *  glIndexPointer
   *  glFogCoordPointer
   *  glTexCoordPointer
   *  glEdgeFlagPointer
   *
   * They all have 4 parameters:
   * -size: the number of components per element
   * -type: the type of data: GL_FLOAT, GL_INT ect.
   * -stide: number of bytes between components
   * -ptr: Pointer to data.
   *
   * And then we can make use of interleaved arrays and use the commands
   *  glInterleavedArrays to specify multiple arrays at once.
   *  -The usage of this command is fairly complex - see official docs/specs
   *
   */ 
  
}


void Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glRotatef(90,1,0,0); //just to get the axis sorted out :D

  //Draw the geometry using arrays
  glDrawArrays(GL_QUADS,0,NRVERTICES);
  /* 
   * Functions to use in drawing arrays: (Please see official documentation for more information on these!)
   * glArrayElement(int i)
   *  -put between glBegin/glEnd 
   * glDrawArrays(mode,startindex,numvertices)
   *  -no glBegin/glEnd!
   *  -mode is what would be in glBegin(mode)
   *  -render a number of vertices
   * glDrawElements(mode,numvertices,type,*indices)
   *  -indices is an array with the indexes in the array we want called
   *  -type is one of GL_INT GL_BYTE GL_SHORT ect.
   *  -same as glDrawArrays but you specify a number of vertices, not a linear region
   * glMultiDrawArrays(mode,*start,*count,num)
   *  -Same as calling glDrawArrays more than once.
   *  -2 arrays of start and count values.
   * glMultiDrawElements(mode,*count,type,**indices,num)
   *  -like multiple calls to glDrawElements
   *  -Crazy I'll say :D
   */

  SDL_GL_SwapBuffers();
}

int main()
{
  cout << "This is Tut7-array1 example running" << endl;

  //Step 1 is to initialize SDL. Absolutely critical that your SDL application starts with this!
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    cout << "Error initializing SDL" << endl;
    return -1;
  }
  //Step 2 is to get a screen/window on which we can do some graphics.

  //create a Videoclass to control the video with
  SDLVideoclass SDLmain(640,480);
  
  if (SDLmain.InitVideo()){
    cout << "Video up and running" << endl;
  }else{
    cout << "Error initializing Video" << endl;
    return 0;
  }

  //get the events up and running
  SDLEventclass SDLevent;

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,0.0,0.2,0.0);
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0,0,-7);

  Setup();

  Uint32 curticks,prevticks;
  curticks = SDL_GetTicks();
  GLfloat rotate=34;

  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();

    prevticks = curticks;
    curticks = SDL_GetTicks();
    rotate += 45*GLfloat(curticks-prevticks)/1000.0;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-7);
    glRotatef(rotate,0,1,0);
    

    Draw();


  }




  SDL_Quit();
  return 0;
}
