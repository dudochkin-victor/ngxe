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

//code for the extension
#define GL_FOG_COORDINATE_SOURCE          0x8450
#define GL_FOG_COORDINATE                 0x8451
typedef void APIENTRY (* glFogCoordf_func) (GLfloat);

glFogCoordf_func glFogCoordfEXT = NULL;


#include <video.h>
#include <event.h>
#include <texture.h>

#define TRUE 1
#define FALSE 0

GLTextureclass *stonewall;


void setup()
{
  //some code to setup the fog coordinate extension (SDL makes this fairly easy!)
  glFogCoordfEXT = (glFogCoordf_func)SDL_GL_GetProcAddress("glFogCoordf");
  if (!glFogCoordfEXT){
    cout << "EXT_fog_coord not supported!!" << endl;
    exit(0);
  }
  /*
    Setup matrixes and stuff for opengl rendering.
    Also setup for simple fog.
  */
  //the first bit is pretty standard by now:

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,1.0,0.0,0.0);  //the same color as the fog  (nice green toxic fog hey :)
  //clearing the backof the window as solid fog colour increases realism!

  //load a texture
  stonewall = new GLTextureclass;
  stonewall->Load("texture/stone.bmp");
  stonewall->SetLinearFilter();

  glEnable(GL_TEXTURE_2D); //enable texturing

  //now setup the fog parameters
  glFogi(GL_FOG_MODE,GL_LINEAR); //default is GL_EXP (other if GL_LINEAR & GL_EXP2)
  glFogf(GL_FOG_DENSITY,1.0); //default is 1.0 but that is way too dense!
  //and the color of the fog:
  GLfloat fogcolor[] = {0.0,1.0,0.0};
  glFogfv(GL_FOG_COLOR,fogcolor); 
  //enable the fog:
  glEnable(GL_FOG);
  
  glFogi(GL_FOG_COORDINATE_SOURCE_EXT,GL_FOG_COORDINATE_EXT);
 

}

void drawbox(GLfloat locx)
{
  stonewall->Activate(); //make sure we use the texture.

  //compute fog coordinate values
  GLfloat fl,fr;  //for left and right side of cube

  //you can tune these values if you like
  fl = ((locx+5)/14)-0.01;
  fr = fl+0.1;

  glBegin(GL_QUADS);
  glColor3f(1.0,1.0,1.0);
  // face at z = -1
  glFogCoordfEXT(fl);
  glTexCoord2f(0,0);glVertex3f(-1,-1,-1); //left-bot
  glTexCoord2f(0,1);glVertex3f(-1,1,-1);//left-top
  glFogCoordfEXT(fr);
  glTexCoord2f(1,1);glVertex3f(1,1,-1);//right-top
  glTexCoord2f(1,0);glVertex3f(1,-1,-1);//right-bot
  //face at z = 1
  glFogCoordfEXT(fl);
  glTexCoord2f(0,0);glVertex3f(-1,-1,1); //left-bot
  glFogCoordfEXT(fr);
  glTexCoord2f(1,0);glVertex3f(1,-1,1);//right-bot
  glTexCoord2f(1,1);glVertex3f(1,1,1);//right-top
  glFogCoordfEXT(fl);
  glTexCoord2f(0,1);glVertex3f(-1,1,1);//left-top
  //face at x = 1
  glFogCoordfEXT(fr);
  glTexCoord2f(0,0);glVertex3f(1,-1,-1); //left-bot
  glTexCoord2f(1,0);glVertex3f(1,1,-1);//right-bot
  glTexCoord2f(1,1);glVertex3f(1,1,1);//right-top
  glTexCoord2f(0,1);glVertex3f(1,-1,1);//left-top
  //face at x = -1
  glFogCoordfEXT(fl);
  glTexCoord2f(0,0);glVertex3f(-1,-1,-1); //left-bot
  glTexCoord2f(0,1);glVertex3f(-1,-1,1);//left-top
  glTexCoord2f(1,1);glVertex3f(-1,1,1);//right-top 
  glTexCoord2f(1,0);glVertex3f(-1,1,-1);//right-bot
  //face at y = -1
  glFogCoordfEXT(fl);
  glTexCoord2f(0,1);glVertex3f(-1,-1,-1); //left-bot
  glFogCoordfEXT(fr);
  glTexCoord2f(1,1);glVertex3f(1,-1,-1);//right-bot
  glTexCoord2f(1,0);glVertex3f(1,-1,1);//right-top
  glFogCoordfEXT(fl);
  glTexCoord2f(0,0);glVertex3f(-1,-1,1);//left-top
  //face at y = 1
  glFogCoordfEXT(fl);
  glTexCoord2f(1,0);glVertex3f(-1,1,-1); //left-bot
  glTexCoord2f(0,0);glVertex3f(-1,1,1);//left-top
  glFogCoordfEXT(fr);
  glTexCoord2f(0,1);glVertex3f(1,1,1);//right-top
  glTexCoord2f(1,1);glVertex3f(1,1,-1);//right-bot

  glEnd();
  

}
void drawback()
{
  //draw a simple background to make the whole thing just a little more believable.
  glDisable(GL_TEXTURE_2D); //dont want texturing
  glDisable(GL_FOG); //neither want fog
  
  glBegin(GL_QUADS);
  glColor3f(0,0,0); glVertex3f(-20,-50,-30); //left-bot
  glColor3f(0,0,0); glVertex3f(-20,50,-30);//left-top
  glColor3f(0,1,0); glVertex3f(20,50,-30);//right-top
  glColor3f(0,1,0); glVertex3f(20,-50,-30);//right-bot
  glEnd();

  //restore states
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_FOG);
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

  setup();
  GLfloat rotx,locx;
  GLfloat distance;
  rotx = 0;
  locx = 0;

  distance = 10;
  Uint32 curticks,prevticks;
  GLfloat timescale;
  curticks = SDL_GetTicks();

  cout << "Move the cube further and closer with the UP/DOWN arrow keys" << endl;
  cout << "Move the cube left/right with the LEFT/RIGHT arrow keys" << endl;
  cout << "Press ESC to quit" << endl;

  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();
    //events:
    //timescale
    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000.0;
    //rotation
    rotx += timescale*60;
    //moving back forth
    if (SDLevent.GetKey(SDLK_UP))
      if (distance < 15)
	distance += timescale*4;
    if (SDLevent.GetKey(SDLK_DOWN))
      if (distance > 6)
	distance -= timescale*4;
    //moving left right
    if (SDLevent.GetKey(SDLK_LEFT))
      if (locx > -10)
	locx -= timescale*4;
    if (SDLevent.GetKey(SDLK_RIGHT))
      if (locx < 10)
	locx += timescale*4;
    
	 


    //graphics:
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //setup modelview matrix:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //draw background
    drawback();

    //move to cube draw location
    glTranslatef(locx,0,-distance);
    glRotatef(rotx,1,0,0);

    drawbox(locx);
    SDL_GL_SwapBuffers();
  }

  SDL_Quit();
  return 0;
}
