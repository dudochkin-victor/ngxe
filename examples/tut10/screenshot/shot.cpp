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
 * Tutorial 10
 * Screenshot example
 */
#include <iostream>
using namespace std;

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <video.h>
#include <event.h>


#define TRUE 1
#define FALSE 0


void drawbox()
{
  //copied this code from the fog1.cpp(tut6) and removed the texcoord commands
  //and modified it slightly 
  glBegin(GL_QUADS); 
  glColor3f(1.0,0.0,0.0);
  // face at z = -1
  glVertex3f(-1,-1,-1); //left-bot
  glVertex3f(-1,1,-1);//left-top
  glVertex3f(1,1,-1);//right-top
  glVertex3f(1,-1,-1);//right-bot
  //face at z = 1
  glColor3f(0.0,1.0,0.0);
  glVertex3f(-1,-1,1); //left-bot
  glVertex3f(1,-1,1);//right-bot
  glVertex3f(1,1,1);//right-top
  glVertex3f(-1,1,1);//left-top
  //face at x = 1
  glColor3f(0.0,0.0,1.0);
  glVertex3f(1,-1,-1); //left-bot
  glVertex3f(1,1,-1);//right-bot
  glVertex3f(1,1,1);//right-top
  glVertex3f(1,-1,1);//left-top
  //face at x = -1
  glColor3f(1.0,1.0,0.0);
  glVertex3f(-1,-1,-1); //left-bot
  glVertex3f(-1,-1,1);//left-top
  glVertex3f(-1,1,1);//right-top 
  glVertex3f(-1,1,-1);//right-bot
  //face at y = -1
  glColor3f(1.0,0.0,1.0);
  glVertex3f(-1,-1,-1); //left-bot
  glVertex3f(1,-1,-1);//right-bot
  glVertex3f(1,-1,1);//right-top
  glVertex3f(-1,-1,1);//left-top
  //face at y = 1
  glColor3f(0.0,1.0,1.0);
  glVertex3f(-1,1,-1); //left-bot
  glVertex3f(-1,1,1);//left-top
  glVertex3f(1,1,1);//right-top
  glVertex3f(1,1,-1);//right-bot

  glEnd();

}

/* This grabs the screen and save it to "screenshot.bmp"
 * Note: Call this before SDL_GL_SwapBuffers()
 */
void saveimage()
{
  /* The only problem with the process is that the opengl frame buffer
   * is upside down relative to the SDL Surface.
   * Solution 1: Flip buffer before saving image (below)
   * Solution 2: Render image upside down (turn the camera upside down)
   */

  SDL_Surface *tmpsurf;
  tmpsurf = SDL_CreateRGBSurface(0,640,480,24,
				 0xff0000,
				 0x00ff00,
				 0x0000ff,0);
  if (!tmpsurf) return;
  SDL_LockSurface(tmpsurf); //this is simply to make sure it's safe to directly access the surface.

  //get the data from the framebuffer:
  glReadPixels(0,0,640,480,GL_RGB,GL_UNSIGNED_BYTE,tmpsurf->pixels);

  //flip the image upside down
  Uint16 pitch = tmpsurf->pitch;
  char *tmp = new char[pitch];
  char *pixels = (char*)tmpsurf->pixels;
  for (int i=0 ; i < 240 ; i++){
    //swap rows (i) and (639-i)
    memmove(tmp,&pixels[i*pitch],pitch);
    memmove(&pixels[i*pitch],&pixels[(479-i)*pitch],pitch);
    memmove(&pixels[(479-i)*pitch],tmp,pitch);
  }
  delete [] tmp; 

  //save the image:
  SDL_SaveBMP(tmpsurf,"screenshot.bmp");
  
}



int main()
{
  cout << "This is Tut10-shot example running" << endl;

  //Step 1 is to initialize SDL. Absolutely critical that your SDL application starts with this!
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    cout << "Error initializing SDL" << endl;
    return -1;
  }
  //Step 2 is to get a screen/window on which we can do some graphics.

  //create a Videoclass to control the video with
  //Do not change resolution for this example :)
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
  glRotatef(-45,1,0,0);
  glRotatef(25,0,0,1);

  
  //draw image:
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawbox();
  
  saveimage();


  //display image to screen;
  SDL_GL_SwapBuffers();

  
  cout << "BMP image saved.. quitting in 1 second" << endl;
  SDL_Delay(1000);

  SDL_Quit();
  return -1;
}
