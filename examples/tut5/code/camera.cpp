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

#include <SDL/SDL.h>
#include <GL/gl.h>
#include "math.h"
#include "camera.h"

#define TRUE 1
#define FALSE 0
#define PI 3.14159265358979323844

Cameraclass::Cameraclass()
{
  tilt = 0;
  swing = 0;
  locx = 5;
  locy = 5;
  locz = 5;
}


int Cameraclass::ProcessEvent()
{
  SDL_keysym *keysym;

  if (SDL_PollEvent(&curevent)){
    //so there is an event:
    //lets process it!
    switch (curevent.type){
    case (SDL_QUIT): 
      quitrequested = TRUE;
      break;
    case (SDL_KEYDOWN): 
      keysym = &(curevent.key.keysym);
      if (keysym->sym == SDLK_ESCAPE)
	quitrequested = TRUE;
      if (keysym->sym == SDLK_F1)
	SDL_WM_GrabInput(SDL_GRAB_ON);
      if (keysym->sym == SDLK_F2)
	SDL_WM_GrabInput(SDL_GRAB_OFF);
      break;
    case (SDL_MOUSEMOTION):
      tilt += GLfloat(curevent.motion.yrel)/5;
      swing += GLfloat(curevent.motion.xrel)/5;
      break;
    }
    return TRUE;
  }else
    return FALSE;
}


void Cameraclass::Rotate()
{
  glRotatef(tilt,1.0,0.0,0.0);
  glRotatef(swing,0.0,1.0,0.0);
}
void Cameraclass::Move()
{
  glTranslatef(-locx,-locy,-locz);
}
void Cameraclass::Update(GLfloat timescale)
{
  //basic movement in 3D space
  //use w,s,a,d keys to move
  GLfloat lx,lz;
  if (GetKey(SDLK_w)){
    //move forward:
    lx = sin(swing*PI/180)*cos(tilt*PI/180);
    lz = cos(swing*PI/180)*cos(tilt*PI/180);
    locx += timescale*lx*10;
    locz -= timescale*lz*10;
    locy -= timescale*sin(tilt*PI/180)*5;
  }
  if (GetKey(SDLK_s)){
    //move backwards:
    lx = sin(swing*PI/180)*cos(tilt*PI/180);
    lz = cos(swing*PI/180)*cos(tilt*PI/180);
    locx -= timescale*lx*10;
    locz += timescale*lz*10;
    locy += timescale*sin(tilt*PI/180)*5;
  }
  if (GetKey(SDLK_a)){
    //strafe left:
    lx = sin((swing-90)*PI/180);
    lz = cos((swing-90)*PI/180);
    locx += timescale*lx*10;
    locz -= timescale*lz*10;
  }
  if (GetKey(SDLK_d)){
    //strafe right:
    lx = sin((swing+90)*PI/180);
    lz = cos((swing+90)*PI/180);
    locx += timescale*lx*10;
    locz -= timescale*lz*10;
  }
}

