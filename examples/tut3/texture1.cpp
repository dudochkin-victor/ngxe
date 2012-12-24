/*
 simple texture example - Tutorial 3 

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


 */
#include <iostream>
using namespace std;
//include the basic header files for SDL and OpenGL
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "video.h"
#include "event.h"


#define TRUE 1
#define FALSE 0


GLuint texture;

int setuptexture()
{
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // cheap scaling when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // cheap scaling when image smalled than texture

  SDL_Surface *surface;
  surface = SDL_LoadBMP("lightning.bmp");
  if (surface == NULL){
    cout << "Error loading texture file: lightning.bmp" << endl;
    return FALSE;
  }

  /*
    The problems with this:
    OpenGL textures must be upside down. So we must flip the image.
    Also we must tell opengl exactly what format the data is in.
   */

  /*
    format:
    we must select GL_RGB GL_BGR GL_RGBA or GL_BGRA
    
    if Amask is 0 we assume there is no alpha channel
    if Bshift is 0 we assume BGR
   */
  GLenum format;
  if (surface->format->Bshift == 0){
    //BGR
    if (surface->format->Amask == 0)
      format = GL_BGR;
    else
      format = GL_BGRA;
  }else{
    //RGB
    if (surface->format->Amask == 0)
      format = GL_RGB;
    else
      format = GL_RGBA;
  }
  GLint intformat;
  if (surface->format->Amask == 0)
    intformat = 3;
  else 
    intformat = 4;
  /*
    There is no need for us to convert the format since Opengl will store the texture internally in its own
    optimized format. And changes are we don't really get it the same as opengl wants it. So its better just to leave
    it up to the drivers.

    Now the datatype
    this is quite complex. Lets hope it works.
    
    Note: this is untested code!
  */
  GLenum datatype;
  if (surface->format->BytesPerPixel >= 3){
    //assumption: if 4 -> includes alpha channels
    datatype = GL_UNSIGNED_BYTE;
  }else if (surface->format->BytesPerPixel == 1){
    datatype = GL_UNSIGNED_BYTE_3_3_2;
  }else{
    //now the 15 or 16-bit crap
    if (surface->format->Amask == 0){
      //no Alpha
      if (surface->format->BitsPerPixel == 16)
	datatype = GL_UNSIGNED_SHORT_5_6_5; //16 bpp
      else
	datatype = GL_UNSIGNED_SHORT_5_5_5_1; //16bpp
    }else{
      //Alpha
      datatype = GL_UNSIGNED_SHORT_4_4_4_4; 
    }
  }
  /*
    To flip the image the way opengl wants it we simply create a new buffer into which we copy
    the image upside down.. or maybe it was upside down?
   */

  Uint8 *buffer = new Uint8[surface->h * surface->pitch];
  Uint8 *rowa = (Uint8 *)surface->pixels;
  Uint8 *rowb = buffer+((surface->h-1) * surface->pitch);

  for (int y=0; y < surface->h; y++){
    memcpy(rowb,rowa,surface->pitch);
    rowa += surface->pitch;
    rowb -= surface->pitch;
  }

  // Loads the image into OpenGL and create a texture:
  glTexImage2D(GL_TEXTURE_2D,0,intformat,surface->w,surface->h,0,
	       format,datatype,buffer);

  //Check for errors:
  if (glGetError() != GL_NO_ERROR){
    cout << "glTexImage2D failed"<< endl;
    return FALSE;
  }

  //enable texturing
  glEnable(GL_TEXTURE_2D);

  //cleaup buffers
  /*
    OpenGL copied the texture data to internal data, most probably in video memory on the graphics card
    So we may destroy these data structures, they no longer have any use.
   */
  delete [] buffer;
  SDL_FreeSurface(surface);
  return TRUE;
}


int main()
{
  cout << "This is Tut3-light1 example running" << endl;

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

  //Step 3 Load and get the texture ready
  if (!setuptexture()){
    cout << "texture loading failed" << endl;
    return -1;
  }

  //some basic GL setup code
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,-3.0);

  //clear screen
  glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  //draw textured QUAD
  glColor3f(1.0,1.0,1.0); //else we tint the texture :)
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);   //Bot-left
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);   //bot-right
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);   //top-right
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);   //top-left
  glEnd();

  //update screen
  SDL_GL_SwapBuffers();

  //main program loop:
  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();
    //actually we just wait for quad
  }

  SDL_Quit();
  return 0;
}
