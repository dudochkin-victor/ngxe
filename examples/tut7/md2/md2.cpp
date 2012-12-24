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
 *
 *  By Heinrich du Toit
 *  
 * A simple md2 model file loader
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

#include <stdio.h> //used for opening and reading the modelfile

#define TRUE 1
#define FALSE 0

//firstly we define some structures to use with loading the model

typedef struct
{
   int magic;
   int version;
   int skinWidth;
   int skinHeight;
   int frameSize;
   int numSkins;
   int numVertices;
   int numTexCoords;
   int numTriangles;
   int numGlCommands;
   int numFrames;
   int offsetSkins;
   int offsetTexCoords;
   int offsetTriangles;
   int offsetFrames;
   int offsetGlCommands;
   int offsetEnd;
} md2_Header_t;

typedef struct
{
  unsigned char vertex[3];
  unsigned char lightNormalIndex;
} md2_TriangleVertex_t;

typedef struct
{
   float scale[3];
   float translate[3];
   char name[16];
  md2_TriangleVertex_t vertices[1];
} md2_Frame_t;

typedef struct
{
   short vertexIndices[3];
   short textureIndices[3];
} md2_Triangle_t;

typedef struct
{
   short s, t;
} md2_TextureCoordinate_t;

typedef struct
{
   float s, t;
   int vertexIndex;
} md2_glCommandVertex_t;


//support structures for the Md2Modelclass
typedef struct
{
  GLenum mode; 
  GLint first;
  GLsizei count;
} md2_glCommand;

typedef class Md2Modelclass{
public:
  int Load(char * filename);
  void Draw();
  int GetNrFrames() {return nrframes;}
  void SetFrame(int framenr);

private:
  md2_glCommand *glcommands;
  int nrcommandsets;
  int nrvertices;
  GLfloat *texcoords;
  int nrframes;
  GLfloat *allvertices; //[nrframes][nrvertices*3]
  GLfloat *curvertices;
}Md2Modelclass;


int Md2Modelclass::Load(char * filename)
{
  int result;
  FILE *mfile; //modelfile
  //open the file
  mfile = fopen(filename,"rb");
  if (mfile == NULL){
    cout << "Error opening file: "<< filename << endl;
    return FALSE;
  }
  //read header and check magic/version
  md2_Header_t header;
  result = fread(&header,sizeof(md2_Header_t),1,mfile);
  if (result != 1){
    cout << "Error reading model header" << endl;
    return FALSE;
  }
  if (header.magic != 0x32504449){
    cout << "Error: This is not a Md2 Model file!" << endl;
    return FALSE;
  }
  if (header.version != 8){
    cout << "Error: Wrong Md2 version number" << endl;
    return FALSE;
  }
  /* Theres alot of information inside the Md2 model but we don't need it all.
   *   We need basicly the frames and the glcommands to render these frames :)
   *
   */
  
  //first load the glCommands list :)
  /*
   * 
   *
   */
  
  //  md2_glCommandVertex_t **commands;
  int *commands = new int[header.numGlCommands]; 
  fseek(mfile,header.offsetGlCommands,SEEK_SET);
  fread(commands,sizeof(int)*header.numGlCommands,1,mfile);
  nrcommandsets = 0;
  nrvertices = 0;
  //loop throught this a few times :)
  //find number of command sets:
  int *command = commands;
  while (*command != 0){
    nrcommandsets++;
    nrvertices += abs(*command);
    command += (abs(*command) * 3)+1;
  }
  //create structures to contain data!
  glcommands = new md2_glCommand[nrcommandsets];
  //create lookup index thingy 
  /* This is used to lookup vertices in the frames data (later)
   *  and then put them in the correct order in the object!
   * 
   * Also create the texture array at the same time!
   */
  texcoords = new GLfloat[nrvertices*2];
  int *vertexlookup = new int[nrvertices];
  int *curvert = vertexlookup;
  GLfloat *curtex = texcoords;
  md2_glCommand *curcommand = glcommands;
  command = commands;
  int counter = 0; //counts vertices
  while (*command != 0){
    if (*command >0){
      curcommand->mode = GL_TRIANGLE_STRIP;
    }else{
      curcommand->mode = GL_TRIANGLE_FAN;
    }
    curcommand->count = abs(*command);
    curcommand->first = counter;
    command++;
    for (int i=0; i < curcommand->count ; i++){
      md2_glCommandVertex_t *vert = (md2_glCommandVertex_t*)(command);
      command += 3; 
      *curtex = vert->s;
      curtex++;
      *curtex = vert->t;
      curtex++;
      *curvert = vert->vertexIndex;
      curvert++;
      counter++;
    }
    curcommand++;
  }
  //  for (int i=0; i < 10 ; i++){
  //    cout << vertexlookup[i] << endl;
  //  }
  /* Load the frames :)
   * We use the information from the glCommands to convert the frames data to something more usefull
   */

  nrframes = header.numFrames;
  //  cout << header.frameSize << endl;
  //  cout << sizeof(md2_Frame_t)+sizeof(md2_TriangleVertex_t)*header.numVertices << endl;
  allvertices = new GLfloat[nrframes*nrvertices*3];
  fseek(mfile,header.offsetFrames,SEEK_SET);
  char *frames = new char[header.numFrames*header.frameSize]; 
  fread(frames,header.frameSize,header.numFrames,mfile);
  
  for (int i=0;i< header.numFrames ; i++){
    curvertices = &allvertices[nrvertices*i*3];
    md2_Frame_t *curframe = (md2_Frame_t *)(&frames[i*header.frameSize]);
    //print out names of frames?
    //curframe->name[15] = 0;
    //cout << i << " = " << curframe->name << endl;
    md2_TriangleVertex_t *framevertices = curframe->vertices;

    for (int j=0 ; j < nrvertices ; j++){
      for (int k=0; k < 3; k++){
	curvertices[k] = GLfloat(framevertices[vertexlookup[j]].vertex[k])*curframe->scale[k]+curframe->translate[k];
      }
      curvertices += 3;
    }
  }
  curvertices = allvertices;

  //done!
  fclose(mfile);
  cout << "model loaded :)" << endl;
  return TRUE;
}
void Md2Modelclass::Draw()
{
  glEnable(GL_VERTEX_ARRAY);
  glEnable(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3,GL_FLOAT,0,curvertices);
  glTexCoordPointer(2,GL_FLOAT,0,texcoords);

  glColor3f(1,1,1);

  for (int i=0; i < nrcommandsets ; i++){
    glDrawArrays(glcommands[i].mode,glcommands[i].first,glcommands[i].count);
  }

  
}
void Md2Modelclass::SetFrame(int framenr)
{
  if ((framenr >= nrframes)||(framenr < 0))
    return;
  curvertices = &allvertices[nrvertices*framenr*3];
}


//global variables:
Md2Modelclass *model;


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
  gluPerspective(45.0f,1.33,0.1f,500.0f);   //setup the camera
   
  glClearColor(0.2,0.0,0.0,0.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-70);
  glRotatef(-90,1,0,0);

  //load model
  model = new Md2Modelclass;
  
  model->Load("goblin.md2");

  //Load and select the texture
  /* This defnitely is not the way to integrate texture handling into models!!!
   */
  glEnable(GL_TEXTURE_2D);
  Textureclass texture1;
  texture1.Load("hobgoblin.bmp");
  texture1.SetLinearFilter();
  texture1.Activate();

  int count = 0;
  int frame = 0;

  cout << "Press ESC to quit" << endl;
  while (!SDLevent.MustQuit()){
    SDLevent.ProcessEvent();

    count++;
    if (count > 100){
      frame++;
      if (frame >= model->GetNrFrames()){
	frame = 0;
      }
      count = 0;
      model->SetFrame(frame);
    }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glRotatef(0.1,0,0,1);

    model->Draw();
    SDL_GL_SwapBuffers();

    //    return 0;
  }




  SDL_Quit();
  return 0;
}
