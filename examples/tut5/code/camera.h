//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit

#ifndef CAMERA_HH
#define CAMERA_HH

#include "event.h"

typedef class Cameraclass:public SDLEventclass{
 public:
  Cameraclass();
  int ProcessEvent(); //returns true if there was an event to process

  
  void Update(GLfloat timescale);
  void Rotate();
  void Move();
  
 protected:
  GLfloat tilt,swing;
  GLfloat locx,locy,locz;
 

}Cameraclass;





#endif
