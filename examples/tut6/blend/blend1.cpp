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

#include <video.h>
#include <event.h>
#include <texture.h>

#define TRUE 1
#define FALSE 0


//textures
GLTextureclass *stone;
GLTextureclass *wash;



void drawbox()
{
  //yet again we draw a 3D cube. Kewl hey!
  //we should really get to models somewhere in the future
  glBegin(GL_QUADS);
  // face at z = -1
  glTexCoord2f(0,0);glVertex3f(-1,-1,-1); //left-bot
  glTexCoord2f(0,1);glVertex3f(-1,1,-1);//left-top
  glTexCoord2f(1,1);glVertex3f(1,1,-1);//right-top
  glTexCoord2f(1,0);glVertex3f(1,-1,-1);//right-bot
  //face at z = 1
  glTexCoord2f(0,0);glVertex3f(-1,-1,1); //left-bot
  glTexCoord2f(1,0);glVertex3f(1,-1,1);//right-bot
  glTexCoord2f(1,1);glVertex3f(1,1,1);//right-top
  glTexCoord2f(0,1);glVertex3f(-1,1,1);//left-top
  //face at x = 1
  glTexCoord2f(0,0);glVertex3f(1,-1,-1); //left-bot
  glTexCoord2f(1,0);glVertex3f(1,1,-1);//right-bot
  glTexCoord2f(1,1);glVertex3f(1,1,1);//right-top
  glTexCoord2f(0,1);glVertex3f(1,-1,1);//left-top
  //face at x = -1
  glTexCoord2f(0,0);glVertex3f(-1,-1,-1); //left-bot
  glTexCoord2f(0,1);glVertex3f(-1,-1,1);//left-top
  glTexCoord2f(1,1);glVertex3f(-1,1,1);//right-top 
  glTexCoord2f(1,0);glVertex3f(-1,1,-1);//right-bot
  //face at y = -1
  glTexCoord2f(0,1);glVertex3f(-1,-1,-1); //left-bot
  glTexCoord2f(1,1);glVertex3f(1,-1,-1);//right-bot
  glTexCoord2f(1,0);glVertex3f(1,-1,1);//right-top
  glTexCoord2f(0,0);glVertex3f(-1,-1,1);//left-top
  //face at y = 1
  glTexCoord2f(1,0);glVertex3f(-1,1,-1); //left-bot
  glTexCoord2f(0,0);glVertex3f(-1,1,1);//left-top
  glTexCoord2f(0,1);glVertex3f(1,1,1);//right-top
  glTexCoord2f(1,1);glVertex3f(1,1,-1);//right-bot

  glEnd();
  

}

//for rotating the cubes:
GLfloat rotx=0,roty=0;
GLfloat timescale;

//does all the drawing:
void drawscene()
{
  //update rotation:
  rotx += timescale*70;
  roty += timescale*35;

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glLoadIdentity(); //reset matrix
  glTranslatef(0,0,-12); //move to visible position
  //rotation:
  glRotatef(rotx,1,0,0);
  glRotatef(roty,0,1,0);

  //draw first cube (inside)
  //this cube is solid stone!
  stone->Activate();
  glColor3f(1,1,1); //solid color
  drawbox();

  //draw the second cube (outside)
  /*
    This cube is bigger than the stone cube and surround it
    But this cube is slightly see throught
    So we switch on blending
   */ 
  //alter the angle  (cubes seem to rotate completely different)
  glRotatef(rotx,0,1,0);
  glRotatef(roty,0,0,1);
  //this cube is bigger: 
  glScalef(2.5,2.5,2.5);

  //enable blending
  glEnable(GL_BLEND);
  //disable depth buffer writing!! 
  glDepthMask(GL_FALSE);

  //note: we should also disbale culling if it was used!
  
  wash->Activate(); //texture select
  
  //this time the colour must have an apha value!
  glColor4f(1,1,1,0.3); //RGBA
  //Alpha of 1.0 is completely solid while 0.0 is completely transparent

  drawbox();

  
  //disable blending again
  glDisable(GL_BLEND);
  //enable depth buffer writing again
  glDepthMask(GL_TRUE);


  SDL_GL_SwapBuffers();
}


int main()
{
  cout << "This is Tut6-fog1 example running" << endl;

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
  glEnable(GL_TEXTURE_2D);

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera

  glClearColor(0.0,0.0,0.3,0.0);  

  glMatrixMode(GL_MODELVIEW); //switch to modelview matrix for the rest of the example

  //load the textures
  stone = new GLTextureclass;
  stone->Load("stone.bmp");
  stone->SetLinearFilter();

  wash = new GLTextureclass;
  wash->Load("wash.bmp");
  wash->SetLinearFilter();

  //setup blending function:
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //timescaling as usual:
  Uint32 curticks,prevticks;
  curticks = SDL_GetTicks();

  while (!SDLevent.MustQuit()){
    //events:
    SDLevent.ProcessEvent();

    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000.0;

    //graphics:
    drawscene();
    
  }

  SDL_Quit();
  return 0;
}
