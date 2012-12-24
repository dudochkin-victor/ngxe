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
/*
 *
 *  By Heinrich du Toit
 *  
 * Vertex arrays example code
 * Tutorial 7
 *
 *
 */


#include <iostream>
using namespace std;

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <video.h>
#include <event.h>
#include <texture.h>

#ifndef GL_VERSION_1_3
#error Need opengl version 1.3 minimum to compile
#endif

#define TRUE 1
#define FALSE 0

//some simple coordinates


Textureclass *texture1;
Textureclass *texture2;


void Setup()
{
  //find out how many texture units are available
  GLint textureunits;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS,&textureunits);
  cout << "Number of texture units present: " << textureunits << endl;

  //we need atleast 2 texture units to make this work
  if (textureunits < 2){
    cout << "This example needs atleast 2 texture units present" << endl; 
    exit(-1);
  }

  //Setup texture 1 on texture unit 0
  glActiveTexture(GL_TEXTURE0); //select unit

  texture1 = new Textureclass;
  texture1->Load("sky1.bmp");
  texture1->SetLinearFilter();
  texture1->Activate(); //BIND

  glEnable(GL_TEXTURE_2D); //activate the texture unit

  //Setup texture 2 on texture unit 1
  glActiveTexture(GL_TEXTURE1);

  texture2 = new Textureclass;
  texture2->Load("sky2.bmp");
  texture2->SetLinearFilter();
  texture2->Activate(); //BIND

  glEnable(GL_TEXTURE_2D);
}

void Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Draw a square on the screen

  //We need to give texture coordinate to both texture units now

  glBegin(GL_QUADS);
  glColor3f(1,1,1);

  glMultiTexCoord2f(GL_TEXTURE0,0,1);
  glMultiTexCoord2f(GL_TEXTURE1,0,0);
  glVertex3f(-1,1,-0.67);


  glMultiTexCoord2f(GL_TEXTURE0,1,1);
  glMultiTexCoord2f(GL_TEXTURE1,0,1);
  glVertex3f(1,1,0.35);


  glMultiTexCoord2f(GL_TEXTURE0,1,0);
  glMultiTexCoord2f(GL_TEXTURE1,1,1);
  glVertex3f(1,-1,0.35);


  glMultiTexCoord2f(GL_TEXTURE0,0,0);
  glMultiTexCoord2f(GL_TEXTURE1,1,0);
  glVertex3f(-1,-1,-0.67);

  glEnd();


  SDL_GL_SwapBuffers();
}


int main()
{
  cout << "This is Tut7-array1 example running" << endl;

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


  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,0.0,0.2,0.0);
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0,0,-5);

  Setup();

  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();

    glRotatef(0.1,0,1,0);

    Draw();

  }


  SDL_Quit();
  return 0;
}
