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
#include <GL/gl.h>
#include "skybox.h"

#define TRUE 1
#define FALSE 0

/*
 This gets the skybox ready for display!
 */
int SkyBox::Load(char* texfilename)
{
  //step 1 - load the texture
  if (!texture.Load(texfilename)){
    return FALSE;
  }
  texture.SetLinearFilter();
  texture.SetWrap(GL_REPEAT);
  //step 2 - Create the display list
  dlist.Compile();
  texture.Activate(); //set the correct texture
  glDepthMask(GL_FALSE); //make sure we don't write on the depth-buffer
  DrawBox(); // draw the sky box
  glDepthMask(GL_TRUE); //restore normal depth-buffer behaviour
  dlist.End();

  //thats it:)
  return TRUE;
}



void SkyBox::DrawBox()
{
  //we draw the box around the location 0,0,0 (where the camera should be)
  //and we must draw it big enough to not get clipped by camera near but also small enough 
  //to not get clipped by camera far clip region

  //It took me some time to get all there coordinates figured out
  //The texture aint really perfectly suited for a skybox but it'll have todo :)
  glBegin(GL_QUADS);
  glColor3f(1.0,1.0,1.0);
  // face at z = -10
  glTexCoord2f(0,0);glVertex3f(-10,-10,-10); //left-bot
  glTexCoord2f(2,0);glVertex3f(10,-10,-10);//right-bot
  glTexCoord2f(2,2);glVertex3f(10,10,-10);//right-top
  glTexCoord2f(0,2);glVertex3f(-10,10,-10);//left-top
  //face at z = 10
  glTexCoord2f(0,0);glVertex3f(-10,-10,10); //left-bot
  glTexCoord2f(0,2);glVertex3f(-10,10,10);//left-top
  glTexCoord2f(2,2);glVertex3f(10,10,10);//right-top
  glTexCoord2f(2,0);glVertex3f(10,-10,10);//right-bot
  //face at x = 10
  glTexCoord2f(0,0);glVertex3f(10,-10,-10); //left-bot
  glTexCoord2f(0,2);glVertex3f(10,-10,10);//left-top
  glTexCoord2f(2,2);glVertex3f(10,10,10);//right-top
  glTexCoord2f(2,0);glVertex3f(10,10,-10);//right-bot
  //face at x = -10
  glTexCoord2f(0,0);glVertex3f(-10,-10,-10); //left-bot
  glTexCoord2f(2,0);glVertex3f(-10,10,-10);//right-bot
  glTexCoord2f(2,2);glVertex3f(-10,10,10);//right-top
  glTexCoord2f(0,2);glVertex3f(-10,-10,10);//left-top
  //face at y = -10
  glTexCoord2f(0,2);glVertex3f(-10,-10,-10); //left-bot
  glTexCoord2f(0,0);glVertex3f(-10,-10,10);//left-top
  glTexCoord2f(2,0);glVertex3f(10,-10,10);//right-top
  glTexCoord2f(2,2);glVertex3f(10,-10,-10);//right-bot
  //face at y = 10
  glTexCoord2f(2,0);glVertex3f(-10,10,-10); //left-bot
  glTexCoord2f(2,2);glVertex3f(10,10,-10);//right-bot
  glTexCoord2f(0,2);glVertex3f(10,10,10);//right-top
  glTexCoord2f(0,0);glVertex3f(-10,10,10);//left-top

  glEnd();
}

