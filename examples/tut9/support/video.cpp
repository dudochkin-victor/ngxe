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
using namespace std; //Dont forget this ;)

//include the basic header files for SDL and OpenGL
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "video.h"

//please see the make file for compilation instructions
#define TRUE 1
#define FALSE 0



SDLVideoclass::SDLVideoclass()
{
  //default contructor startup settings. 640x480 windowed mode.
  width = 640;
  height = 480;
  bpp = 16;
  flags = SDL_OPENGL;
  screen = NULL;
}
SDLVideoclass::SDLVideoclass(int w,int h)
{
  //contructor with custom width and height settings. (still windowed mode)
  width = w;
  height = h;
  bpp = 16;
  flags = SDL_OPENGL;
  screen = NULL;
}
SDLVideoclass::~SDLVideoclass()
{
  //nothing to cleanup 
}
int SDLVideoclass::InitVideo()
{

  //first check if video subsystem is active
  if (!SDL_WasInit(SDL_INIT_VIDEO)){
    //try to initialize video subsystem if not active
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0){
      cout << "Unable to initialize SDL video sub-system: " << SDL_GetError() << endl;
      return FALSE;
    }
  }

  //get the video information to obtain the best Pixel format from that
  const SDL_VideoInfo *videoinfo = SDL_GetVideoInfo();
  //The best video format for the current hardware will be in: videoinfo->vfmt
  bpp = (videoinfo->vfmt->BitsPerPixel);
  
  //check if the current settings will work.
  int newbpp = SDL_VideoModeOK(width,height,bpp,flags);
  if (!newbpp){
    //this will completely not work. This is very unlikely.
    cout << "Requested video mode not available..." << endl;
    //try to find a working mode:
    if (!FindMode())
      return FALSE;
  }else if (newbpp != bpp){
    // switch to a different pixel layout: 
    //(Since we got bpp from VideoInfo this shouldn't really ever happend)
    cout << "Requested Bits-per-pixel not available. Switching to " << newbpp << " bpp." << endl;
    bpp = newbpp;
  }

  //This configures some setting about the OpenGL configuration.

  //THE Colour attributes are calculated from the videoinfo obtained
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8-videoinfo->vfmt->Rloss);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8-videoinfo->vfmt->Gloss);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8-videoinfo->vfmt->Bloss);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1); //enable GL double buffering
  
  //action:
  if ((screen = SDL_SetVideoMode(width,height,bpp,flags)) == NULL){
    cout << "Error setting Video Mode: " << SDL_GetError() << endl;
    return FALSE;
  }
  
  //setup GL window:
  glViewport(0,0,width,height);

  //debugging code:
  cout << "Video mode up and running: ";
  cout << screen->w << "x" << screen->h << "@" << int(screen->format->BitsPerPixel);
  if (screen->flags & SDL_FULLSCREEN)
    cout << " (Fullscreen)";
  cout << endl;

  return TRUE; //everything successfull
}
void SDLVideoclass::SetWidth(int w)
{
  width = w;
}
void SDLVideoclass::SetHeight(int h)
{
  height = h;
}
int SDLVideoclass::FindMode()
{
  //Finds the closest mode.
  SDL_Rect **modes;
  int i;
  //get the modes available:
  //the NULL tels SDL we are only intrested in the modes that apply to the pixelformat we get from SDL_GetVideoInfo.
  modes = SDL_ListModes(NULL,flags);
  if (!modes){ //this is not 2 good. Maybe fullscreen is not possible here?
    cout << "There is no valid modes for the given configuration." << endl;
    return FALSE;
  }
  if (modes == (SDL_Rect**)-1){
    //this is likely to happen if you are in windowed mode. 
    //call the fullscreen function before this function.
    cout << "No restriction on the current configuration. All resolutions valid." << endl;
    return TRUE;
  }
  //ok find a suitable mode
  /*
    The algorithm is to calculate the difference in the number of pixels on the screen
    and then find the resolution closest to that requisted.
    This should work in most cases.
    A method for finding the biggest resolution:
      Set width and height to like 5000 each and call FindMode.
   */
  int aimsize;
  int bestsize;
  int cursize;
  aimsize = width*height; 
  bestsize = 0;
  for (i=0; modes[i]; ++i){
    cursize = modes[i]->w * modes[i]->h;
    if (abs(aimsize-cursize) < abs(aimsize-bestsize)){
      bestsize = cursize;
      width = modes[i]->w;
      height = modes[i]->h;
    }
  }
  if (bestsize == 0){
    //this means some completely wacky mode was chosen or the graphics hardware is close to non-functional.
    cout << "No available mode even close to wanted resolution" << endl;
    return FALSE;
  }
  cout << "Found fitting resolution of " << width << "x" << height << endl;
  return TRUE;
}
void SDLVideoclass::Fullscreen()
{
  //simply add the fullscreen flag
  flags |= SDL_FULLSCREEN;
}
void SDLVideoclass::Windowed()
{
  //remove the fullscreen flag
  flags &= ~SDL_FULLSCREEN;
  //this is fairly straigth forward binary logic
}
void SDLVideoclass::ShutdownVideo()
{
  //shutdown the video system.
  //dont worry you can call InitVideo again after this, it will restart the video system.
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
