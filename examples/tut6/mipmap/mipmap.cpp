/*
 * By Heinrich du Toit
 *
 * Mipmap example - Tutorial 6
 *
 * Note: The texture is 256x256 to make sure minification is mostly used.
 * If your implementation can not handle textures of this size simply load 
 * sunset.bmp in your favourite image editor and resize to 128x128 or 64x64.
 * All OpenGL implementations should be able to handle 64x64 sized bitmaps.
 */
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

#include <video.h>
#include <event.h>


typedef class Textureclass{
 public:
  Textureclass();
  ~Textureclass();

  //call before Load (NOT AFTER!)
  void Mipmap(); 
  void NoMipmap(); //default;

  int Load(const char* filename);

  void Activate() {glBindTexture(GL_TEXTURE_2D,texture);}

  //other options

  void SetWrap(GLint mode);

  //call after Load to setup the filter settings.
  void SetLinearFilter();
  void SetNearestFilter();
  
 protected:
  GLuint texture;
  int mipmap;

}Textureclass;


#define TRUE 1
#define FALSE 0

Textureclass::Textureclass()
{
  glGenTextures(1,&texture);
  mipmap = FALSE;
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

  // Loads the image into OpenGL and create a texture:
  if (mipmap){
    //The glu library will do all the hard work for us
    cout << "Creating Mipmaps "<< endl;
    gluBuild2DMipmaps(GL_TEXTURE_2D,intformat,surface->w,surface->h,format,datatype,buffer);
    //and as easy as that we have a mipmapped texture
  }else{
    cout << "Creating normal texture" << endl;
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

void Draw()
{
  glBegin(GL_QUADS);
  glColor3f(1,1,1);
  glTexCoord2f(0,0);glVertex3f(-3,-3,0); //left-bot
  glTexCoord2f(0,1);glVertex3f(-3,3,0);//left-top
  glTexCoord2f(1,1);glVertex3f(3,3,0);//right-top
  glTexCoord2f(1,0);glVertex3f(3,-3,0);//right-bot
  glEnd();
}


int main()
{
  cout << "This is Tut6-mipmap example running" << endl;

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

  //some gl setup parameters
  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.
  
  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
  glClearColor(0.0,0.0,0.1,0.0);  //the same color as the fog  (nice green toxic fog hey :)

  //from now on use the modelview matrix for manipulation!
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_TEXTURE_2D);

  //get the events up and running
  SDLEventclass SDLevent;

  //setup texture:
  Textureclass texture;
  //comment out this line to disable Mipmapping and see how the scene looks without it!
  texture.Mipmap();

  if (!texture.Load("sunset.bmp")){
    cout << "texture load error, halting" << endl;
    return -1;
  }
  texture.SetLinearFilter();
  texture.Activate(); //use this texture and this texture only!

  GLfloat scale=1.2;
  Uint32 curticks,prevticks;
  curticks = SDL_GetTicks();
  
  cout << "press L to select LINEAR filtering"<< endl;
  cout << "press N to select NEAREST filtering" << endl;
  cout << "press ESC to quit" << endl;

  while (!SDLevent.MustQuit()){
    //events
    if (SDLevent.ProcessEvent()){
      if (SDLevent.GetLastEvent()->type == SDL_KEYDOWN){
	if (SDLevent.GetLastEvent()->key.keysym.sym == SDLK_l){
	  cout << "Selecting Linear texture filtering" << endl;
	  texture.SetLinearFilter();
 	}
	if (SDLevent.GetLastEvent()->key.keysym.sym == SDLK_n){
	  cout << "Selecting Nearest texture filtering" << endl;
	  texture.SetNearestFilter();
	}
      }
    }

    //calculate the rotation value
    prevticks = curticks;
    curticks = SDL_GetTicks();
    scale -= scale*0.7*GLfloat(curticks-prevticks)/1000.0;
    if (scale < 0.10) scale = 1.2;
    //graphics
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0,0,-15);
    glScalef(scale,scale,scale);
    Draw();

    //draw the thing at tight angles 
    glLoadIdentity();
    glTranslatef(5,0,-15);
    glRotatef(80,0,1,0);
    Draw();
    glLoadIdentity();
    glTranslatef(0,4.5,-15);
    glRotatef(90,1,0,0);
    Draw();
    glLoadIdentity();
    glTranslatef(-5.5,-4.0,-15);
    glRotatef(90,-0.707,0.707,0);
    Draw();

    SDL_GL_SwapBuffers();
  }

  return 0;
}
