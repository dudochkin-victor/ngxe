/*
 View - Tutorial 2 
 Basic MODELVIEW transformations: scaling / rotating / translating

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

//include our 2 new classes to be used.
#include "video.h"
#include "event.h"




void info()
{
  cout << "controls" << endl;
  cout << "Y axis scaling: A and S" << endl;
  cout << "X axis translating: U and I" << endl;
  cout << "Y axis translating: J and K" << endl;
  cout << "tilting: UP and DOWN" << endl;
  cout << "Swing: Z and X" << endl;
  cout << "Roll: LEFT and RIGHT" << endl;
  cout << "Press ESC to quit or close window" << endl;
}

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

int main()
{
  cout << "This is Tut2-View example running" << endl;

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

  SDLEventclass SDLevent;

  //Step 3 Setup the PROJECTION matrix
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
  
  //ok select the MODELVIEW matrix. we will be working with this for the rest of the time.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,-6.0);

  //some other setup code:
  glShadeModel(GL_SMOOTH); //colour fill technique
  glClearColor(0.0,0.0,0.3,0.0); //the background color
  
  glEnable(GL_DEPTH_TEST);

  //variables for translation
  GLfloat tilt,roll,swing;
  GLfloat scaley;
  GLfloat transx,transy;

  tilt = 0.0;
  roll = 0.0;
  swing = 0.0;
  scaley = 1.0;
  transx = 0.0;
  transy = 0.0;
  
  Uint32 ticksold,ticksnew;
  GLfloat timescale;

  info();
  //Main program loop:
  ticksnew = SDL_GetTicks();
  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();
    //main loop part 1 - events
    //calulate scaling factor according to time.
    ticksold = ticksnew;
    ticksnew = SDL_GetTicks();
    timescale = GLfloat(ticksnew-ticksold)/1000; //timescale of 1.0 = 1 second has elapsed
    //check if the scaling keys A or S was pressed
    if (SDLevent.GetKey(SDLK_a)){
      scaley += timescale*0.4;
    }
    if (SDLevent.GetKey(SDLK_s)){
      scaley -= timescale*0.4;
      if (scaley < 0.1) scaley = 0.1; //dont make it too small
    }
    //check if the translation keys was pressed.
    if (SDLevent.GetKey(SDLK_u))
      transx += timescale*2;
    if (SDLevent.GetKey(SDLK_i))
      transx -= timescale*2;
    if (SDLevent.GetKey(SDLK_j))
      transy += timescale*2;
    if (SDLevent.GetKey(SDLK_k))
      transy -= timescale*2;

    //check if the "tilt" keys was pressed
    if (SDLevent.GetKey(SDLK_UP))
      tilt += timescale*60; //60 degrees per second
    if (SDLevent.GetKey(SDLK_DOWN))
      tilt -= timescale*60; //60 degrees per second
    //check the "swing" keys
    if (SDLevent.GetKey(SDLK_z))
      swing += timescale*60; //60 degrees per second
    if (SDLevent.GetKey(SDLK_x))
      swing -= timescale*60; //60 degrees per second
    //check the "roll" keys
    if (SDLevent.GetKey(SDLK_LEFT))
      roll += timescale*60; //60 degrees per second
    if (SDLevent.GetKey(SDLK_RIGHT))
      roll -= timescale*60; //60 degrees per second


    //main loop part 2 - graphics
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear
    //modelview matrix setup
    glLoadIdentity(); //start with the identity matrix
    //translate the piramid

    glTranslatef(transx,transy,-12.0); //we move the whole thing away from the camera so that we can actually see it.

    // rotate the piramid
    //the x,y,z order:
    glRotatef(tilt,1.0,0.0,0.0); //tilt
    glRotatef(swing,0.0,1.0,0.0); //swing
    glRotatef(roll,0.0,0.0,1.0); //roll
    
    //The y,x,z order: 
    /*
    glRotatef(roll,0.0,1.0,0.0);//turning
    glRotatef(tilt,1.0,0.0,0.0); //bending back
    glRotatef(swing,0.0,0.0,1.0);
    */
    //scale the piramid
    glScalef(1.0,scaley,1.0);


    drawpiramid(); //geometry
    SDL_GL_SwapBuffers(); //update
  }
  SDL_Quit(); //SDL shutdown
  return 0; 

}


