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
 *  By Heinrich du Toit
 *
 * Automatic texture coordinate generation
 *  Tutorial 9
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


#define TRUE 1
#define FALSE 0


void drawbox()
{
  //copied this code from the fog1.cpp(tut6) and removed the texcoord commands
  //and modified it slightly 
  glBegin(GL_QUADS);
  glColor3f(1.0,1.0,1.0);
  // face at z = -1
  glVertex3f(-1,-1,-1); //left-bot
  glVertex3f(-1,1,-1);//left-top
  glVertex3f(1,1,-1);//right-top
  glVertex3f(1,-1,-1);//right-bot
  //face at z = 1
  glVertex3f(-1,-1,1); //left-bot
  glVertex3f(1,-1,1);//right-bot
  glVertex3f(1,1,1);//right-top
  glVertex3f(-1,1,1);//left-top
  //face at x = 1
  glVertex3f(1,-1,-1); //left-bot
  glVertex3f(1,1,-1);//right-bot
  glVertex3f(1,1,1);//right-top
  glVertex3f(1,-1,1);//left-top
  //face at x = -1
  glVertex3f(-1,-1,-1); //left-bot
  glVertex3f(-1,-1,1);//left-top
  glVertex3f(-1,1,1);//right-top 
  glVertex3f(-1,1,-1);//right-bot
  //face at y = -1
  glVertex3f(-1,-1,-1); //left-bot
  glVertex3f(1,-1,-1);//right-bot
  glVertex3f(1,-1,1);//right-top
  glVertex3f(-1,-1,1);//left-top
  //face at y = 1
  glVertex3f(-1,1,-1); //left-bot
  glVertex3f(-1,1,1);//left-top
  glVertex3f(1,1,1);//right-top
  glVertex3f(1,1,-1);//right-bot

  glEnd();
  

}


int main()
{
  cout << "This is Tut9-texgen1 example running" << endl;

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

  
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,0.0,0.2,0.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-6);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


  //setup texturing:
  Textureclass fire;
  fire.Mipmap();
  fire.Load("fire.bmp");
  fire.SetLinearFilter();
  fire.Activate();
  glEnable(GL_TEXTURE_2D);


  //texture coordinate generation:

  glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
  glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
  GLfloat planes[] = {1,1,0};
  GLfloat planet[] = {1,0,1};
  glTexGenfv(GL_S,GL_OBJECT_PLANE,planes);
  glTexGenfv(GL_T,GL_OBJECT_PLANE,planet);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);


  Uint32 curticks,prevticks;
  curticks = SDL_GetTicks();
  GLfloat timescale;

  GLfloat rot1,rot2;
  rot1 = 0;
  rot2 = 20;

  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();

    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000.0;

    rot1 += timescale*90;
    rot2 += timescale*45;
    glLoadIdentity();
    glTranslatef(0,0,-6);
    glRotatef(rot1,1,0,0);
    glRotatef(rot2,0,1,0);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    drawbox();

    SDL_GL_SwapBuffers();
  }

 

  SDL_Quit();
  return 0;
}
