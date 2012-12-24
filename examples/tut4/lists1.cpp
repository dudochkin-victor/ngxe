/*
 display lists example -  Tutorial 4 

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



GLuint displaylist1;

//our kewl piramid drawing code from tut2
void drawpiramid()
{

  //the top of the piramid
  glBegin(GL_TRIANGLE_FAN);
  glColor3f(1.0,1.0,1.0);
  glVertex3f(0.0,2.0,0.0);

  glColor3f(1.0,0.0,0.0);
  glVertex3f(-2.0,-1.0,-2.0);
  glColor3f(0.0,1.0,0.0);
  glVertex3f(-2.0,-1.0,2.0);
  glColor3f(0.0,0.0,1.0);
  glVertex3f(2.0,-1.0,2.0);
  glColor3f(1.0,1.0,0.0);
  glVertex3f(2.0,-1.0,-2.0);
  glColor3f(1.0,0.0,0.0);
  glVertex3f(-2.0,-1.0,-2.0);
  glEnd();
  //the bottom of the piramid
  glBegin(GL_QUADS);
  glColor3f(1.0,0.0,0.0);
  glVertex3f(-2.0,-1.0,-2.0);
  glColor3f(0.0,1.0,0.0);
  glVertex3f(-2.0,-1.0,2.0);
  glColor3f(0.0,0.0,1.0);
  glVertex3f(2.0,-1.0,2.0);
  glColor3f(1.0,1.0,0.0);
  glVertex3f(2.0,-1.0,-2.0);
  glEnd();
}


void makelist()
{
  //step 1 - create a new display list to be used
  displaylist1 = glGenLists(1); //create 1 empty usable display list

  //start to compile the new display list
  glNewList(displaylist1,GL_COMPILE); //tell opengl that we want to compile/make a new list and put the commands in display list 1

  //now what we want in the list is the commands to draw our piramid.
  drawpiramid();
 
  //ok end the list
  glEndList();
  
  //the list displaylist1 should be ready for usage now!

}

int main()
{
  //The first part of this code will by now be pretty standard to you:

  cout << "This is Tut4-lists1 example running" << endl;

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


  //some GL setup routines
  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera

  //setup modelview matrix:
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 
  glClearColor(0.0,0.0,0.2,0.0); 


  //Ok lets start with the display list:
  makelist(); 

  GLfloat rot = 0.0;

  Uint32 curticks,prevticks;
  GLfloat timescale;
  curticks = SDL_GetTicks();
  
  cout << "Use Left and Right to rotate piramids" << endl;
  //main loop:
  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();
    //main loop 1 - events
    //timescale:
    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000.0;

    //check rotation keys
    if (SDLevent.GetKey(SDLK_LEFT))
      rot += timescale*90;
    if (SDLevent.GetKey(SDLK_RIGHT))
      rot -= timescale*90;

    //main loop 2 - graphics
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //we draw 2 piramids:

    //piramid 1: to the left
    glLoadIdentity();
    glTranslatef(-5.0,0.0,-20.0); 
    glRotatef(rot,0.0,1.0,0.0);
    //call the display list to draw the piramid rather than the procedure.
    glCallList(displaylist1);

    //piramid 2: to the right
    glLoadIdentity();
    glTranslatef(5.0,0.0,-20.0); 
    glRotatef(-rot,0.0,1.0,0.0);
    //call the display list to draw the piramid rather than the procedure.
    glCallList(displaylist1);

    
    SDL_GL_SwapBuffers();
  }


  SDL_Quit();
  return 0;
}
