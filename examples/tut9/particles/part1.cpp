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
 * A small example to show the basic of mechanical physics (Newton)
 *  Tutorial 7
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


typedef class Particleclass{
public:
  Particleclass(GLfloat x,GLfloat y,GLfloat z,GLfloat totlife);

  int Go(GLfloat timescale); //do render and move and everything in one shot (return false if dead)
  

  
private:
  GLfloat life; 
  GLfloat pos[3];
  GLfloat rot;

}Particleclass;

Particleclass::Particleclass(GLfloat x,GLfloat y,GLfloat z,GLfloat totlife)
{
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
  life = totlife;
  rot = GLfloat(random())/GLfloat(RAND_MAX/360);
}
int Particleclass::Go(GLfloat timescale)
{
  life -= timescale;
  if (life < 0) return FALSE;
  rot += timescale*GLfloat(random())/GLfloat(RAND_MAX/90);
  
  glPushMatrix();
  glTranslatef(pos[0],pos[1],pos[2]);
  glRotatef(rot,0,0,1);

  glColor4f(1,1,1,life);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(0,0,0);
  glTexCoord2f(0,1);
  glVertex3f(0,0.1,0);
  glTexCoord2f(1,1);
  glVertex3f(0.1,0.1,0);
  glTexCoord2f(1,0);
  glVertex3f(0.1,0,0);
  glEnd();
  glPopMatrix();
  return TRUE;
}

#define MAXPARTICLES 250
typedef class Enginclass{
public:
  Enginclass();
  
  void Go(GLfloat timescale);

  //  void SetRadius

private:
  int nrparticles;
  Particleclass *particles[MAXPARTICLES];
  GLfloat pnt[3];
  GLfloat rot;

}Enginclass;
Enginclass::Enginclass()
{
  rot = 0;
  nrparticles = 0;
  pnt[0] = 2*sin(rot);
  pnt[1] = 2*cos(rot);
  pnt[0] = 1;
  for (int i=0; i < MAXPARTICLES ; i++){
    GLfloat a;
    a = GLfloat(random())/GLfloat(RAND_MAX);
    GLfloat life;
    life = GLfloat(random())/GLfloat(RAND_MAX/2);
    particles[i] = new Particleclass(pnt[0]*a,pnt[1]*a,pnt[2]*a,life);
  }
  nrparticles = MAXPARTICLES;
}
void Enginclass::Go(GLfloat timescale)
{
  rot += timescale * 0.6;
  pnt[0] = 2*sin(rot);
  pnt[1] = 2*cos(rot);

  GLfloat dx,dy;
  dx = 0.1*sin(rot+90);
  dy = 0.1*cos(rot+90);
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glColor4f(0.5,0.5,1,1);
  glVertex3f(0,0,0);
  glVertex3f(0+dx,0+dy,0);
  glVertex3f(pnt[0],pnt[1],pnt[2]);
  glVertex3f(pnt[0]+dx,pnt[1]+dy,pnt[2]);
  glEnd();
  glEnable(GL_TEXTURE_2D);
  
  GLfloat a;
  a = GLfloat(random())/GLfloat(RAND_MAX);
  GLfloat life;
  life = GLfloat(random())/GLfloat(RAND_MAX);
  if (nrparticles < MAXPARTICLES){
    particles[nrparticles] = new Particleclass(pnt[0]*a,pnt[1]*a,pnt[2]*a,life);
    nrparticles++;
  }
  for (int i=nrparticles-1; i >= 0; i--){
    if (!particles[i]->Go(timescale)){
      nrparticles--;
      particles[i] = particles[nrparticles];
    }
  }
}



int main()
{
  cout << "This is Tut7-phys1 example running" << endl;

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

  //dont need depth buffer for this example
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,0.0,0.2,0.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-6);

  glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);


  Textureclass particle;
  particle.Load("particle.bmp");
  particle.SetLinearFilter();
  glEnable(GL_TEXTURE_2D);

  Enginclass sword;


  
  Uint32 curticks,prevticks;
  curticks = SDL_GetTicks();
  GLfloat timescale;
    


  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();
    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000.0;


    glClear(GL_COLOR_BUFFER_BIT);
    sword.Go(timescale);
    SDL_GL_SwapBuffers();
  }



  SDL_Quit();
  return 0;
}
