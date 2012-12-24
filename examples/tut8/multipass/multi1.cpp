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
#include <light.h>


#define TRUE 1
#define FALSE 0

//some simple coordinates
GLfloat square[] = {-1,1,-0.67,
		    1,1,0.35,
		    1,-1,0.35,
		    -1,-1,-0.67};


void Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //BIG RED SQUARE
  glColor3f(1,0,0);
  glEnable(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_FLOAT,0,square);
  glDrawArrays(GL_QUADS,0,4);

  
  // move everything 1 unit closer in z buffer:
  glPolygonOffset(0,-1);

  //find interpolated z locations
  GLfloat z1,z2;
  z1 = -0.67+(0.35+0.67)*0.2;
  z2 = -0.67+(0.35+0.67)*0.8;

  //Small blue square ontop of RED Square
  glColor3f(0,0,1);
  glBegin(GL_QUADS);
  glVertex3f(-0.6,0.2,z1);
  glVertex3f(0.6,0.2,z2);
  glVertex3f(0.6,-0.2,z2);
  glVertex3f(-0.6,-0.2,z1);
  glEnd();

  //return default settings
  glPolygonOffset(0,0);


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


  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,0.0,0.2,0.0);
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0,0,-5);


  glEnable(GL_DEPTH_TEST); //enable depth buffer
  //simple method:
  glDepthFunc(GL_LEQUAL);

  //better method:
  glEnable(GL_POLYGON_OFFSET_FILL); //comment out this line to see the effect

  glPolygonOffset(0,0); //defaults


  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();

    glRotatef(0.1,0,1,0);

    Draw();

  }


  SDL_Quit();
  return 0;
}
