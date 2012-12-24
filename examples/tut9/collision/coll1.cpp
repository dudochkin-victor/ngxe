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
  GLfloat GetPosition(GLint i){return pos[i];}
  GLfloat GetVelocity(GLint i){return vel[i];}
  GLfloat GetAcceleration(GLint i){return acc[i];}

  //force calculation
  void ResetForce() {acc[0] = 0;acc[1] = 0;acc[2] = 0;}
  void AddForce(GLfloat fx,GLfloat fy,GLfloat fz);
  void AddForce(GLfloat *f){AddForce(f[0],f[1],f[2]);}
  void AddGravity(){acc[1] -= 9.8;} //add gravitation force in the negative y axis direction 


  //update frame
  void CalcVel(GLfloat timescale);
  void Update(GLfloat timescale); //calculate new vel and loc

  //to make the opengl a bit easier:
  void Translate(){glTranslatef(pos[0],pos[1],pos[2]);}

protected:
  GLfloat mass;
  GLfloat pos[3]; //location/position
  GLfloat vel[3]; //velocity/speed
  GLfloat acc[3]; //acceleration

} Objectclass;

Objectclass::Objectclass()
{
  mass = 1; //defualt value :)
  pos[0] = 0;pos[1] = 0; pos[2] = 0;
  vel[0] = 0;vel[1] = 0; vel[2] = 0;
  acc[0] = 0;acc[1] = 0; acc[2] = 0;
}
void Objectclass::SetPosition(GLfloat x,GLfloat y,GLfloat z)
{
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
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
void Objectclass::CalcVel(GLfloat timescale)
{
  vel[0] += acc[0]*timescale;
  vel[1] += acc[1]*timescale;
  vel[2] += acc[2]*timescale;
}
void Objectclass::Update(GLfloat timescale)
{
  //this is the action ;)
  pos[0] += vel[0]*timescale;
  pos[1] += vel[1]*timescale;
  pos[2] += vel[2]*timescale;
}

//forward declaration
typedef class Planeclass;

typedef class Sphereclass: public Objectclass{
public:
  //constructor
  Sphereclass():radius(0){}
  
  //setup parameters
  void SetRadius(GLfloat r){radius = r;}
  //read parameters
  GLfloat GetRadius(){return radius;}
  void SetElasticity(GLfloat e){elastic = e;}
  GLfloat GetElasticity(){return elastic;}

  //collision with other sphere
  int CheckCollision(Sphereclass &s,GLfloat deltatime);
  GLfloat GetColtime(){return coltime;}
  int HandleCollision(Sphereclass &s);
 

  //collision with plane
  int CheckCollision(Planeclass &p,GLfloat deltatime);
  int CheckCollision2(Planeclass &p,GLfloat deltatime);
  int HandleCollision(Planeclass &p);

protected:
  GLfloat radius;
  GLfloat elastic; //factor
  
  //collision data:
  GLfloat coltime;


}Sphereclass;


int Sphereclass::CheckCollision(Sphereclass &s,GLfloat deltatime)
{
  //note: s is also Sphereclass so we can access variables directly!

  GLfloat deltapos[3];
  GLfloat deltavel[3];
  for (int k=0; k < 3; k++){ 
    //the compiler will unroll this loop for us if optimization enabled
    deltapos[k] = pos[k]-s.pos[k];
    deltavel[k] = vel[k]-s.vel[k];
  }
  
  GLfloat a,b,c; //for the quadratic equation
  a = deltavel[0]*deltavel[0]+deltavel[1]*deltavel[1]+deltavel[2]*deltavel[2];
  if (abs(a) < 0.000001) return FALSE; //spheres moving 2 slowly
  b = 2*deltavel[0]*deltapos[0]+2*deltavel[1]*deltapos[1]+2*deltavel[2]*deltapos[2];
  c = deltapos[0]*deltapos[0]+deltapos[1]*deltapos[1]+deltapos[2]*deltapos[2];
  c -= (radius+s.radius)*(radius+s.radius); //what this should equal 2

  GLfloat delta;
  delta = b*b - 4*a*c;
  if (delta < 0) return FALSE; //no collision ever with current speeds/positions

  delta = sqrt(delta);
  a = 2*a;

  GLfloat dt,dt1;
  dt = (-b + delta)/(a);
  dt1 = (-b - delta)/(a);
  if (dt1 < dt) dt = dt1;

  //put in some exstra margins of 1% outside time boundary
  if ((dt >= -0.01)&&(dt <= (deltatime*1.01))){
    //OK we have a collision!
    coltime = dt;
    s.coltime = dt;
    return TRUE;
  }
  return FALSE;
}
int Sphereclass::HandleCollision(Sphereclass &s)
{
  GLfloat normal[3];
  for (int k=0; k< 3; k++)
    normal[k] = s.pos[k]-pos[k];
  //normalize
  GLfloat normalsize;
  normalsize = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  for (int k=0; k< 3; k++)
    normal[k] /= normalsize;
  
  //calculate the normal velocities
  //and remove them from the objects current velocity state
  GLfloat vel1[3]; //this sphere
  GLfloat vel2[3]; //other sphere
  for (int k=0; k< 3; k++){
    vel1[k] = vel[k]*normal[k];
    vel[k] *= (1-fabs(normal[k]));
    vel2[k] = s.vel[k]*(-normal[k]);
    s.vel[k] *= (1-fabs(normal[k]));
  }
  //calculate normal velocity
  GLfloat nvel1,nvel2;
  nvel1 = sqrt(vel1[0]*vel1[0]+vel1[1]*vel1[1]+vel1[2]*vel1[2]);
  nvel2 = sqrt(vel2[0]*vel2[0]+vel2[1]*vel2[1]+vel2[2]*vel2[2]);
  //now its a 1D collision :)

  //calculate final velocity
  GLfloat fvel1,fvel2;
  fvel1 = nvel1*(mass-elastic*s.mass)/(mass+s.mass)
    +nvel2*(elastic+1)*(s.mass)/(mass+s.mass);
  fvel2 = nvel2*(s.mass-s.elastic*mass)/(mass+s.mass)
    +nvel1*(s.elastic+1)*(mass)/(mass+s.mass);

  //add velocities back into object
  for (int k=0; k< 3; k++){
    vel[k] -= fvel1*(normal[k]);
    s.vel[k] += fvel2*(normal[k]);
  }
  
  return TRUE;
}


typedef class Planeclass{
public:
  friend class Sphereclass;

  void SetCoords(GLfloat *c);
  GLfloat GetCoord(int i){return coords[i];}
  const GLfloat* GetCoords(){return coords;}

  

private:
  GLfloat coords[3];
  GLfloat ncoords[3];//normalized

}Planeclass;
void Planeclass::SetCoords(GLfloat *c)
{
  if (!c) return;
  coords[0] = c[0];
  coords[1] = c[1];
  coords[2] = c[2];
  GLfloat size;
  size = sqrt(coords[0]*coords[0]+coords[1]*coords[1]+coords[2]*coords[2]);
  ncoords[0] = coords[0]/size;
  ncoords[1] = coords[1]/size;
  ncoords[2] = coords[2]/size;
}


//global variables
GLfloat timescale;

Sphereclass ball1;
Sphereclass ball2;
Planeclass planes[6];

GLUquadric *sphere;

int Sphereclass::CheckCollision(Planeclass &p,GLfloat deltatime)
{
  //calculate the position on the edge of the sphere that is going to hit the plane
  GLfloat epos[3];
  for (int k=0;k < 3 ; k++){
    epos[k] = pos[k] + p.ncoords[k]*radius;
  }
  //check movement direction
  GLfloat dot1=0;
  for (int k=0; k < 3 ; k++){
    dot1 += p.ncoords[k]*vel[k];
  }
  if (dot1 <= 0) return FALSE;

  //we need to set the (-p.coords) dot (epos-p.coords) = 0 to solve the time problem
  GLfloat a,b;
  a=0;  b=0;
  for (int k=0; k < 3 ; k++){
    a += (-p.coords[k])*vel[k];
    b += (-p.coords[k])*(epos[k]-p.coords[k]);
  }
  if (b == 0){
    coltime = 0;
    return TRUE;
  }
  GLfloat dt;
  dt = -b/a;
  
  if ((dt >= -0.01)&&(dt <= (deltatime*1.01))){
    //    cout << p.coords[0] << endl;    
    //    cout << (epos[0]-p.coords[0])*(-p.coords[0]) << " "<< epos[1]-p.coords[1] << " " << epos[2]-p.coords[2] << endl;
    //    cout << a << "  "<< b << endl;
    //OK we have a collision!
    coltime = dt;
    return TRUE;
  }
  return FALSE;
}
int Sphereclass::CheckCollision2(Planeclass &p,GLfloat deltatime)
{
  //slightly faster method
  //calculate the position on the edge of the sphere that is going to hit the plane
  GLfloat epos[3];
  for (int k=0;k < 3 ; k++){
    epos[k] = pos[k] + p.ncoords[k]*radius;
  }
  GLfloat dot1=0,dot2=0;
  for (int k=0;k < 3 ; k++){
    dot1 += (epos[k]-p.coords[k])*(-p.coords[k]);
    dot2 += (epos[k]+timescale*vel[k]-p.coords[k])*(-p.coords[k]);
  }
  coltime = 0;
  //the fabs tests is because if one of the points is so close it's just as good as a collision
  if ((fabs(dot1) < 0.001)||(fabs(dot2)< 0.001)||(dot1*dot2 <= 0)){
    //actually need to calculate correct coltime!

    return TRUE;
  }
  return FALSE;
}
int Sphereclass::HandleCollision(Planeclass &p)
{
  //get velocity components
  GLfloat vel1[3];
  vel1[0] = vel[0]*p.ncoords[0];
  vel1[1] = vel[1]*p.ncoords[1];
  vel1[2] = vel[2]*p.ncoords[2];
  //remove normal velocity 
  vel[0] *= (1-fabs(p.ncoords[0]));
  vel[1] *= (1-fabs(p.ncoords[1]));
  vel[2] *= (1-fabs(p.ncoords[2]));

  //calculate normal velocity = 1D problem
  GLfloat veln;
  veln = sqrt(vel1[0]*vel1[0]+vel1[1]*vel1[1]+vel1[2]*vel1[2]);
  veln *= elastic;
  //add back resulting velocity
  for (int k=0; k< 3; k++){
    vel[k] += veln*(-p.ncoords[k]);
  }
  return TRUE;
}


void drawscene()
{
  //update physics on ball

  ball1.ResetForce();
  ball1.AddGravity();

  ball2.ResetForce();
  ball2.AddGravity();

  ball1.CalcVel(timescale);
  ball2.CalcVel(timescale);
  
  if (ball1.CheckCollision(ball2,timescale)){
    //move balls to perfect position

    //this creates problems:
    //timescale = ball1.GetColtime();
    //    ball1.Update(timescale);
    //    ball2.Update(timescale);
    //handle the collision now:
    ball1.HandleCollision(ball2);
    //    cout << "Sphere-Sphere Collision" << endl;
  }
  
  for (int k=0; k < 6; k++)
    if (ball1.CheckCollision(planes[k],timescale)){
      //these commands are removed because they give errors
      //      timescale = ball1.GetColtime();
      //      ball1.Update(timescale);
      //      cout << "Sphere-Plane Collision1" << endl;
      ball1.HandleCollision(planes[k]);
    }

  for (int k=0; k < 6; k++)
    if (ball2.CheckCollision2(planes[k],timescale)){
      //these commands are removed because they give errors
      //      timescale = ball2.GetColtime();
      //      ball2.Update(timescale);
      //cout << "Sphere-Plane Collision2" << endl;
      ball2.HandleCollision(planes[k]);
    }


  //continue normal bahaviour
  ball1.Update(timescale);
  ball2.Update(timescale);

  //go graphics
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-10);

  glPushMatrix();
  ball1.Translate();
  //draw the little ball
  glColor3f(1,0,0); //red ball :)
  gluSphere(sphere,ball1.GetRadius(),36,18); 
  glPopMatrix();
  //the second ball:
  glPushMatrix();
  ball2.Translate();
  glColor3f(0.5,0.5,1); //light blue ball :)
  gluSphere(sphere,ball2.GetRadius(),36,18); 
  glPopMatrix();
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
  ball1.SetMass(5); 
  ball1.SetPosition(-3,1,-1);
  ball1.SetVelocity(6.5,-15,8);
  ball1.SetRadius(0.35);
  ball1.SetElasticity(0.95);

  ball2.SetMass(5);
  ball2.SetPosition(2.5,0,0);
  ball2.SetVelocity(-6,7.1,-10);
  ball2.SetRadius(0.55);
  ball2.SetElasticity(0.95);

  GLfloat pcoords0[] = {-1,-2,0};
  planes[0].SetCoords(pcoords0);
  GLfloat pcoords1[] = {2,-1,0};
  planes[1].SetCoords(pcoords1);
  GLfloat pcoords2[] = {0,0,2};
  planes[2].SetCoords(pcoords2);
  GLfloat pcoords3[] = {0,0,-2};
  planes[3].SetCoords(pcoords3);
  GLfloat pcoords4[] = {1,3,0};
  planes[4].SetCoords(pcoords4);
  GLfloat pcoords5[] = {-2,1,0};
  planes[5].SetCoords(pcoords5);


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
