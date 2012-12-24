/*
 Simple lighting example - Tutorial 3

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
#include "math.h"



void setuplight()
{
  //setup light properties:
  //these are actually the defaults but I include it here for you to play with
  GLfloat ambient[] = {0.0,0.0,0.0,1.0};
  GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
  GLfloat specular[] = {1.0,1.0,1.0,1.0};

  glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,specular);

  //total world ambient light
  GLfloat worldambient[] = {0.1,0.1,0.1,1.0}; 
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,worldambient);

  glEnable(GL_LIGHT0); //enable usage of light0
  glEnable(GL_LIGHTING); //enable lighting in opengl


  //the other 2 material properties:
  
  GLfloat matspecular[] = {1.0,1.0,1.0,1.0};
  GLfloat matemission[] = {0.0,0.0,0.0,1.0};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,matspecular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,matemission);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,100); //must be between 0 and 128

  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  //now color will control the material properties.

}



int main()
{
  cout << "This is Tut3-light1 example running" << endl;

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
  glTranslatef(0.0,-1.0,-10.0); //moves geometry to a visible position in front of the camera
 
  glClearColor(0.0,0.0,0.2,0.0); 

  setuplight(); 

  //the time scale stuff:
  Uint32 curticks,prevticks;
  GLfloat timescale;
  curticks = SDL_GetTicks();

  //the rotating light variables:
  GLfloat rotate_light;
  rotate_light = 0;
  GLfloat lightpos[4];
  lightpos[1] = 4.0; //the Y co-ordinate of the light
  lightpos[3] = 1.0; //the w co-ordinate of the light
  //setup the sphere to be drawn
  GLUquadric * sphere = gluNewQuadric();
  
  //the main program loop 
  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();
    //no custom events in this example.

    //timescale:
    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000;
    //move the light around the sphere

    rotate_light += timescale*2.0; //this is in radians
    lightpos[0] = 5.0*cos(rotate_light); //calculate the x co-ordinate
    lightpos[2] = 5.0*sin(rotate_light); //calculate the z co-ordinate
    glLightfv(GL_LIGHT0,GL_POSITION,lightpos); //update the light position

    
    //main loop part 2 - graphics
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //draws the sphere
    glColor3f(0.6,1.0,0.6); //sets the DIFFUSE AND AMBIENT prioperties of the material
    gluSphere(sphere,2.0,72,36);
    SDL_GL_SwapBuffers();
  }
  cout << "Byebye" << endl;
  SDL_Quit();
  return 0;
}

