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

 */
#include <iostream>
using namespace std;

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "texture.h"

#define TRUE 1
#define FALSE 0

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3



Textureclass::Textureclass()
{
  glGenTextures(1,&texture);
  mipmap = FALSE;
  compress = FALSE;
}
Textureclass::~Textureclass()
{
  glDeleteTextures(1,&texture);
}
void Textureclass::Mipmap()
{
  mipmap = TRUE;
} 
void Textureclass::NoMipmap()
{
  mipmap = FALSE;
}
void Textureclass::Compress()
{
  compress = TRUE;
}
void Textureclass::NoCompress()
{
  compress = FALSE;
}
int Textureclass::Load(const char* filename)
{
  //active and set initial filtering values
  glBindTexture(GL_TEXTURE_2D,texture);

  SDL_Surface *surface;
  surface = SDL_LoadBMP(filename);
  if (surface == NULL){
    cout << "Error loading texture file: " << filename << endl;
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

  if (compress){
    //all we need todo to compress the texture is to change the internal format
    if (intformat == 3){
      //there is a very good change this format is supported
      intformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
    }else{
      //intformat == 4 assumed
      intformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
      //this is the simplest RGBA format and most likely available!
    }
  }
 
  // Loads the image into OpenGL and create a texture:
  if (mipmap){
    //The glu library will do all the hard work for us
    gluBuild2DMipmaps(GL_TEXTURE_2D,intformat,surface->w,surface->h,format,datatype,buffer);
    //and as easy as that we have a mipmapped texture
  }else{
    glTexImage2D(GL_TEXTURE_2D,0,intformat,surface->w,surface->h,0,
		 format,datatype,buffer);
  }

  //Check for errors:
  if (glGetError() != GL_NO_ERROR){
    cout << "glTexImage2D failed"<< endl;
    return FALSE;
  }
  //make sure atleast some filters are selected:
  SetNearestFilter();

  //cleaup buffers
  delete [] buffer;
  SDL_FreeSurface(surface);
  return TRUE;
}


void Textureclass::SetWrap(GLint mode)
{
  Activate();
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,mode);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,mode);

}
void Textureclass::SetLinearFilter()
{
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  if (mipmap){
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    //GL_LINEAR_MIPMAP_LINEAR produces the best quality image with the worst performance
  }else{
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  }
}
void Textureclass::SetNearestFilter()
{
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  if (mipmap){
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
  }else{
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  }
}



