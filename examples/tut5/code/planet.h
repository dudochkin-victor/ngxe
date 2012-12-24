//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit

#ifndef PLANET_HH
#define PLANET_HH

#include "texture.h"
#include "lists.h"

extern void InitPlanets();


typedef class Planetclass{
 public:
  Planetclass();

  int LoadTexture(char* filename);
  //set the prioperties of the planets (distance/size/turnspeed/revolvspeed)
  void Set(GLfloat d,GLfloat s,GLfloat ts,GLfloat rs);

  void Update(GLfloat timescale);
  void Draw();

 private:
  GLfloat size,distance;
  GLfloat revolv,turn;
  GLfloat turnspeed,revolvspeed;

  GLTextureclass texture;
  static GLListsclass *dlist;
  

}Planetclass;

#endif
