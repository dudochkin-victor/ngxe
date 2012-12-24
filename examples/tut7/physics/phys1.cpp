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

For header files (which is included in other files at time of compilation) only the following notice is inserted:

This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit

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
#include <light.h>


#define TRUE 1
#define FALSE 0

/*
 * A very simple class to use for an object with mass/velocity and forces acting in on it
 *
 */
typedef class Objectclass{
public:
  Objectclass();


  //directly alter settings!
  void SetMass(GLfloat m) {mass = m;}
  void SetPosition(GLfloat x,GLfloat y,GLfloat z);
  void SetVelocity(GLfloat vx,GLfloat vy,GLfloat vz);
  void SetAcceleration(GLfloat ax,GLfloat ay,GLfloat az);

  //get state
  GLfloat GetMass() {return mass;}
  GLfloat GetPosition(GLint i){return loc[i];}
  GLfloat GetVelocity(GLint i){return vel[i];}
  GLfloat GetAcceleration(GLint i){return acc[i];}

  //force calculation
  void ResetForce() {acc[0] = 0;acc[1] = 0;acc[2] = 0;}
  void AddForce(GLfloat fx,GLfloat fy,GLfloat fz);
  void AddForce(GLfloat *f){AddForce(f[0],f[1],f[2]);}
  void AddGravity(){acc[1] -= 9.8;} //add gravitation force in the negative y axis direction 

  //update frame
  void Update(GLfloat timescale); //calculate new vel and loc

  //to make the opengl a bit easier:
  void Translate(){glTranslatef(loc[0],loc[1],loc[2]);}

protected:
  GLfloat mass;
  GLfloat loc[3]; //location/position
  GLfloat vel[3]; //velocity/speed
  GLfloat acc[3]; //acceleration

} Objectclass;

Objectclass::Objectclass()
{
  mass = 1; //defualt value :)
  loc[0] = 0;loc[1] = 0; loc[2] = 0;
  vel[0] = 0;vel[1] = 0; vel[2] = 0;
  acc[0] = 0;acc[1] = 0; acc[2] = 0;
}
void Objectclass::SetPosition(GLfloat x,GLfloat y,GLfloat z)
{
  loc[0] = x;
  loc[1] = y;
  loc[2] = z;
}
void Objectclass::SetVelocity(GLfloat vx,GLfloat vy,GLfloat vz)
{
  vel[0] = vx;
  vel[1] = vy;
  vel[2] = vz;
}
void Objectclass::SetAcceleration(GLfloat ax,GLfloat ay,GLfloat az)
{
  acc[0] = ax;
  acc[1] = ay;
  acc[2] = az;
}
void Objectclass::AddForce(GLfloat fx,GLfloat fy,GLfloat fz)
{
  //F=ma
  acc[0] += fx/mass;
  acc[1] += fy/mass;
  acc[2] += fz/mass;
}
void Objectclass::Update(GLfloat timescale)
{
  //this is the action ;)
  vel[0] += acc[0]*timescale;
  vel[1] += acc[1]*timescale;
  vel[2] += acc[2]*timescale;
  loc[0] += vel[0]*timescale;
  loc[1] += vel[1]*timescale;
  loc[2] += vel[2]*timescale;
}


//global variables
GLfloat timescale;

Objectclass ball;
GLUquadric *sphere;


void drawscene()
{
  //update physics on ball

  ball.ResetForce();
  ball.AddGravity();
  //Wind maybe?
  ball.AddForce(-25,0,0);
  ball.Update(timescale);
  //go graphics
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-10);

  ball.Translate();
  //draw the little ball
  glColor3f(1,0,0); //red ball :)
  gluSphere(sphere,0.4,36,18); 

  SDL_GL_SwapBuffers();

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

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,0.0,0.2,0.0);
  
  //enable lighting to improve seen depth idea
  GLLightclass light;
  
  light.EnableLight();
  GLfloat pos[] = {5,5,0};
  light.SetPosition(pos);
  GLfloat ambient[] = {0.3,0.3,0.3};
  light.SetAmbient(ambient);
  GLfloat spec[] = {0.7,0.7,0.7};
  light.SetSpecular(spec);

  glEnable(GL_LIGHTING);

  //some material stuff
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  GLfloat matspec[] = {1.0,0.8,0.8};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,matspec);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);


  //setup ball:
  ball.SetMass(5); 
  ball.SetPosition(-4,0,0);
  ball.SetVelocity(10,7,-5);
  

  sphere = gluNewQuadric();

  Uint32 curticks,prevticks;
  curticks = SDL_GetTicks();
  //main program loop
 
  cout << "Press ESC to quit!"  << endl;
  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();

    //timescale calculation 
    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000.0;


    drawscene();
    
  }


  SDL_Quit();
  return 0;
}
