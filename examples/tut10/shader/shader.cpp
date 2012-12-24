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

/*
 * We need to include the Cg runtime library!
 * Check makefile for exstra linking options
 * Please note the usage of upper/lower case in the names!
 */
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include <video.h>
#include <event.h>


#define TRUE 1
#define FALSE 0

/* Some global functions for using CG 
 * call CGShader::Init() at startup
 * call CGShader::Enable* to enable shaders
 */

namespace CGShader{
  CGcontext context;
  CGprofile vertexprofile;
  CGprofile fragmentprofile;

  //Setup context and profiles. Call at start of program once
  int Init();
  //Enable a shader profile (You still need to Bind a program!)
  void EnableVertex() {cgGLEnableProfile(vertexprofile); }
  void EnableFragment() {cgGLEnableProfile(fragmentprofile); }
  //Disable a shader profile (After this the fixed pipeline will be used again)
  void DisableVertex() {cgGLDisableProfile(vertexprofile); }
  void DisableFragment() {cgGLEnableProfile(fragmentprofile); }
  //Call before quitting program or to shutdown Cg
  void Shutdown();

};

int CGShader::Init()
{
  //Step 1: create a global context to work with
  context = cgCreateContext();
  if (!cgIsContext(context)){
    cout << "Error creating CG context" << endl;
    return FALSE;
  }
  //Step 2: get a good profile for the vertex shader:
  vertexprofile = cgGLGetLatestProfile(CG_GL_VERTEX);
  if (vertexprofile == CG_PROFILE_UNKNOWN){
    cout << "Error creating vertex shader profile" << endl;
    return FALSE;
  }
  cgGLSetOptimalOptions(vertexprofile);
  //Step 3: get a good profile for the fragment shader:
  fragmentprofile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  if (fragmentprofile == CG_PROFILE_UNKNOWN){
    cout << "Error creating fragment shader profile" << endl;
    return FALSE;
  }
  cgGLSetOptimalOptions(fragmentprofile);

  //Just for safety we check if any errors occured:
  CGerror error;
  if ((error = cgGetError()) != CG_NO_ERROR){
    cout << "CG Error: " << cgGetErrorString(error) << endl;
    return FALSE;
  }
 
  return TRUE;
}

void CGShader::Shutdown()
{
  cgGLDisableProfile(vertexprofile);
  cgGLDisableProfile(fragmentprofile);
  cgDestroyContext(context);
}

/* A simple class to be a framework for using a shader.
 * (The idea is to build ontop of this class)
 */
typedef class CGShaderclass{
public:
  //contructor and destructor:
  CGShaderclass(){program = NULL;}
  ~CGShaderclass(){cgDestroyProgram(program);}
  //Load a program from a file
  int Load(char * filename,CGGLenum shadertype);
  //Bind/Activate this shader!
  void Bind(){cgGLBindProgram(program);}


  //In some cases you might need to recompile
  
  
protected:
  CGprogram program;

}CGShaderclass;



int CGShaderclass::Load(char * filename,CGGLenum shadertype)
{
  CGprofile profile;
  switch (shadertype){
  case(CG_GL_VERTEX):
    profile = CGShader::vertexprofile;
    break;
  case(CG_GL_FRAGMENT):
    profile = CGShader::fragmentprofile;
    break;
  default:
    cout << "Unknown shader type" << endl;
    return FALSE;
  }
  program = cgCreateProgramFromFile(CGShader::context,CG_SOURCE,
				    filename,profile,
				    NULL,NULL); //entry will be main()
  if (!program){
    CGerror error = cgGetError();
    cout << "Error creating program: " << cgGetErrorString(error) << endl;
    return FALSE;
  }
  //Load program (so that we may bind after this)
  cgGLLoadProgram(program);
  CGerror error;
  if ((error = cgGetError()) != CG_NO_ERROR){
    cout << "Error loading program: " << cgGetErrorString(error) << endl;
    return FALSE;
  }

  return TRUE;
}

/* The Waveclass will be our vertex shader for this example
 *
 */

class WaveShaderclass: public CGShaderclass{
public:
  int Load();
  
  void UpdateState(GLfloat timescale);
  
private:
  CGparameter modelviewproj;
  CGparameter Time;
  GLfloat curtime;

} WaveShader;

int WaveShaderclass::Load()
{
  if (!CGShaderclass::Load("wave.cg",CG_GL_VERTEX))
    return FALSE;

  modelviewproj = cgGetNamedParameter(program,"ModelViewProj");
  if (!modelviewproj){
    cout << "Error: Can't find Parameter" << endl;
    return FALSE;
  }
  Time = cgGetNamedParameter(program,"Time");
  if (!Time){
    cout << "Error: Can't find Parameter" << endl;
    return FALSE;
  }
  curtime = 0;
  return TRUE;
}
void WaveShaderclass::UpdateState(GLfloat timescale)
{
  cgGLSetStateMatrixParameter(modelviewproj,CG_GL_MODELVIEW_PROJECTION_MATRIX,CG_GL_MATRIX_IDENTITY); 

  curtime += timescale;
  cgGLSetParameter1f(Time,curtime);
}

GLfloat timescale;

void Draw()
{
  WaveShader.UpdateState(timescale);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i=-20; i < 20 ; i++){
    glBegin(GL_TRIANGLE_STRIP);
    for (int j=-20;j < 20 ; j++){
      glVertex3f(i*0.2,0,j*0.2);
      glVertex3f(i*0.2+0.2,0,j*0.2);
    }
    glEnd();
  }  
  
  SDL_GL_SwapBuffers();
}

int main()
{
  cout << "This is Tut10-shader example running" << endl;

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

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.
  //  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
  gluLookAt(0,4,8,0,0,0,0,1,0);

  glClearColor(0.0,0.0,0.2,0.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (CGShader::Init()){
    cout << "CG initialized :)" << endl;
  }else{
    return -1;
  }


  if (WaveShader.Load()){
    cout << "Wave shader loaded" << endl;
  }else{
    return -1;
  }
  
  //enable shaders
  CGShader::EnableVertex();
  WaveShader.Bind();


  SDLEventclass event;

  Uint32 curticks,prevticks;
  curticks = SDL_GetTicks();
  
  while (!event.MustQuit()){
    event.ProcessEvent();

    prevticks = curticks;
    curticks = SDL_GetTicks();
    timescale = GLfloat(curticks-prevticks)/1000.0;
    
    //rotation ;)
    glRotatef(30*timescale,0,1,0);
    
    Draw();

  }

  CGShader::Shutdown();
  SDL_Quit();
  return 0;
}
