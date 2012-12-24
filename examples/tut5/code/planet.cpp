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


#include "planet.h"


GLListsclass *Planetclass::dlist = NULL;




Planetclass::Planetclass()
{
  if (dlist == NULL){
    cout << " Creating planet display list" << endl;
    //must create the planet display list :)
    dlist = new GLListsclass;

    GLUquadric *sphere = gluNewQuadric();
    gluQuadricTexture(sphere,GL_TRUE);//generate texture co-ordinates
    dlist->Compile();
    glColor3f(1.0,1.0,1.0);
    gluSphere(sphere,1.0,36,18);
    dlist->End();
    gluDeleteQuadric(sphere);
  }
  distance = 10;
  size = 1;
  revolv = 0;
  turn = 0;
  revolvspeed = 10;
  turnspeed = 45;
}

int Planetclass::LoadTexture(char* filename)
{
  int result;
  result = texture.Load(filename);
  if (result)
    texture.SetLinearFilter();
  return result;
}
void Planetclass::Set(GLfloat d,GLfloat s,GLfloat ts,GLfloat rs)
{
  distance = d;
  size = s;
  turnspeed = ts;
  revolvspeed = rs;
}
void Planetclass::Update(GLfloat timescale)
{
  revolv += revolvspeed*timescale;
  turn += turnspeed*timescale;
}
void Planetclass::Draw()
{
  glPushMatrix(); //save matrix state

  glRotatef(revolv,0.0,1.0,0.0); //revolving around the centre angle.
  glTranslatef(distance,0.0,0.0); //move away from centre into orbit
  glRotatef(turn,0.0,1.0,0.0); //rotate around the planets own axis
  glRotatef(90,1.0,0.0,0.0); //the planets axis straight up.
  glScalef(size,size,size); //The planets size.


  texture.Activate(); //set the texture

  dlist->Execute(); //draw the sphere


  glPopMatrix(); //restore matrix state
}

