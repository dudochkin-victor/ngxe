//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit


#ifndef VIDEO_HH
#define VIDEO_HH

#include <SDL/SDL.h>

/*
  This is a class created to control the video setup.
 */
typedef class SDLVideoclass{
public:
  //contructors
  SDLVideoclass();
  SDLVideoclass(int w,int h);
  //destructor
  ~SDLVideoclass();
  //This will initialize the video system by calling SDL_SetVideoMode
  int InitVideo();
  //these 2 functions can be used to specify the video attributes when not specified with the contructor;
  void SetWidth(int w);
  void SetHeight(int h);
  //find the closest hardware available video mode. This is useless with windowed mode!
  int FindMode();
  //Set to use fullscreen display mode.
  void Fullscreen();
  //set to use a windowed display (default)
  void Windowed();
  //Shut down the video display. (All other SDL sub-systems remains active)
  void ShutdownVideo();
  //retrieve the current settings.
  int GetWidth() {return width;}
  int GetHeight() {return height;}
  int GetBpp() {return bpp;}
  int GetFlags() {return flags;}
private:
  //private variables
  int width;
  int height;
  int bpp;
  int flags;
  SDL_Surface *screen;
} SDLVideoclass;


#endif
