/*
 Solar system demo - Tutorial 5
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
#include "camera.h"
#include "error.h"
#include "lists.h"
#include "texture.h"
#include "light.h"
#include "material.h"

#include "skybox.h"
#include "planet.h"

#define TRUE 1
#define FALSE 0


SkyBox *sky;

Cameraclass *camera;
Planetclass *planets;

GLLightclass *star;

Uint32 curticks,prevticks;




void DrawScene()
{
  GLfloat timescale;
  prevticks = curticks;
  curticks = SDL_GetTicks();
  timescale = GLfloat(curticks-prevticks)/1000.0;

  camera->Update(timescale);

  //clear depth buffer:
  glClear(GL_DEPTH_BUFFER_BIT);

  //reset the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); //reset MODELVIEW Matrix

  //start by setting up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
  

  //rotate camera
  camera->Rotate(); //rotate camera for skybox :)
  
  //disable lighting for skybox:
  glDisable(GL_LIGHTING);
  //Now draw the SkyBox
  sky->Draw();
  //enable ligthing again:
  glEnable(GL_LIGHTING);

  //move the camera
  camera->Move();
  
  glMatrixMode(GL_MODELVIEW);
  //Identity already loaded.

  
  //draw the star:
  /* Since the light source is inside the star the light does not light up the star.
     So we give the star some emission to become visible.
   */
  GLfloat star_emission[] = {1.0,0.5,0.5,1.0}; //a reddish glo
  glMaterialfv(GL_FRONT,GL_EMISSION,star_emission);
  planets[0].Update(timescale);
  planets[0].Draw();
  GLfloat planet_emission[] = {0.0,0.0,0.0,1.0};
  glMaterialfv(GL_FRONT,GL_EMISSION,planet_emission);

  //draw the planets 
  for (int i = 1;i < 4;i++ ){
    planets[i].Update(timescale);
    planets[i].Draw();
  }

  //update screen:
  SDL_GL_SwapBuffers();
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
  //  SDLmain.Fullscreen();
  //  SDLmain.FindMode();
  
  if (SDLmain.InitVideo()){
    cout << "Video up and running" << endl;
  }else{
    cout << "Error initializing Video" << endl;
    return 0;
  }

  camera = new Cameraclass;
  SDL_ShowCursor(SDL_DISABLE);

  //some GL setup routines
  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //enable culling:
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glEnable(GL_TEXTURE_2D);
 
  glClearColor(0.0,0.0,0.2,0.0); 


  //Load skybox:
  sky = new SkyBox;
  if (!sky->Load("textures/sky.bmp")){
    cout << "Error creating skybox" << endl;
    return -1;
  }
  //Load planets:
  planets = new Planetclass[4]; //4 planets
  planets[0].LoadTexture("textures/star.bmp");
  planets[0].Set(0,1.3,40,0);
  planets[1].LoadTexture("textures/planet1.bmp");
  planets[1].Set(4,0.3,15,15);
  planets[2].LoadTexture("textures/planet2.bmp");
  planets[2].Set(8,0.9,120,20);
  planets[3].LoadTexture("textures/planet3.bmp");
  planets[3].Set(13,0.6,90,17);

  curticks = SDL_GetTicks(); //just get a valid curticks value before starting

  //Setup lighting
  star = new GLLightclass(GL_LIGHT0);
  GLfloat starpos[] = {0.0,0.0,0.0};
  star->SetPosition(starpos);
  star->EnableLight();
  glEnable(GL_LIGHTING);


  //main loop:
  while (!camera->MustQuit()){
    camera->ProcessEvent();
    
    DrawScene();
  }
  //cleanup
  delete [] planets;
  delete sky;
  delete camera;
  
  SDL_Quit();
  return 0;
}
