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

#include <stdlib.h>

#define TRUE 1
#define FALSE 0


#define PACK __attribute__ ((packed))

// blender/python does not pack data!
typedef struct SHeader{
  unsigned char id;
  char name[6];
  int nrobjects;
}SHeader; 
typedef struct SObjIndex{
  int type;
  int offset;
}SObjIndex;
typedef struct SIndex{
  unsigned char id;
  int offset;
}SIndex;
#define MAXINDEX 32
typedef struct SStart{
  unsigned char id;
  unsigned char objectnr;
  int count;
}SStart;
typedef struct SVertex{
  int index;
  float co[3];
  float no[3];
  float uvco[2];
}SVertex;
typedef struct SFace{
  char nrvertices;
  int vertices[4];
  float normal[3];
  float uv[8];
}SFace;
typedef struct SObjInfo{
  unsigned char id;
  float loc[3];
  float rot[3];
  float size[3];
  float matrixWorld[16];
}SObjInfo;

#define CAMERA 01
#define MESH 02
#define LAMP 03

/*
 Because there can be more than one "object" inside blender at once
 And one object (e.g. a robot) can consists of multiple "objects" 
 So we need to load all and keep them seperated.
 We will later sort out what todo with this data
 */

typedef class SObjectclass{
public:
  void SetType(int t){type = t;}
  int GetType(){return type;}

  int Load(FILE *f);
  void Draw();

private:
  int LoadVertices(FILE *f);
  int LoadFaces(FILE *f);
  int LoadInfo(FILE *f);

  int type;
  SObjInfo *info;
  int nrvertices;
  SVertex *vertex;
  int nrfaces;
  SFace *face;

}SObjectclass;
/* A simple function to draw the object if of type MESH
 */
void SObjectclass::Draw()
{
  //is mesh?
  if (type != MESH) return; 
  cout << "Drawing object" << endl;
  glPushMatrix(); 

  //Apply Modelview matrix transformation to move object to correct location in world:
  glMultMatrixf(info->matrixWorld);

  //Loop to display every face
  for (int i=0; i < nrfaces ; i++){
    //We do not know if the face is a tri or quad so we use GL_POLYGON...
    glBegin(GL_POLYGON); //lame but it works ;)
    //Call information for every vertex in the face
    for (int j=0; j < face[i].nrvertices; j++){
      SVertex *v = &vertex[face[i].vertices[j]];
      glVertex3fv(v->co);
      glNormal3fv(v->no);
    }
    glEnd();
  }

  //restore Modelview matrix:
  glPopMatrix();
}

/* This object is a container for multiple objects
 */
typedef class SimpleObjectclass{
public:
  int Load(char * filename);
  void Draw();
  int GetNrObjects(){return nrobjects;}
  SObjectclass *GetObject(int nr){return &objects[nr];}

private:

  int nrobjects;
  SObjectclass *objects;


}SimpleObjectclass;

/*
 * This is the main Load function for the simple file format.
 * 
 */
int SimpleObjectclass::Load(char * filename)
{
  //Open the file:
  FILE *f;
  f = fopen(filename,"rb");
  if (!f) return FALSE;
  //Read the Header:
  SHeader header;
  fread(&header,sizeof(SHeader),1,f);
  //Check if this is the correct file type
  if (strncmp("simple",header.name,6)){
    cout << "This is not a simple object file!" << endl;
    return FALSE;
  }
  if (header.id != 102){
    cout << "File has wrong id (possibly wrong version)" << endl;
    return FALSE;
  }
  //Load the objects index 
  SObjIndex *objs = new SObjIndex[header.nrobjects];
  fread(objs,sizeof(SObjIndex),header.nrobjects,f);
  //Load objects:
  //(Using the object index to locate the objects)
  cout << (int)header.nrobjects << "objects in file" << endl;
  nrobjects = header.nrobjects;
  objects = new SObjectclass[header.nrobjects];
  
  //This Loop calls SObjectclass::Load once for every object
  //The results is that all objects gets loaded :)
  for (int i=0; i < nrobjects ; i++){
    objects[i].SetType(objs[i].type);
    fseek(f,objs[i].offset,SEEK_SET);
    if (!objects[i].Load(f))
      return FALSE;
  }

  return TRUE;
}
/* This load the data for 1 object.
 * This function gets called from SimpleObjectclass::Load 
 * (The file-marker is already positioned at the start of the object data)
 */
int SObjectclass::Load(FILE *f)
{
  //Load indexes:
  //Currently there is a maximum number of indexes: MAXINDEX
  //And all of them is always available. The last one is mark with a 0 in the id field.
  SIndex index[MAXINDEX];
  int cur = 0;
  do{
    fread(&index[cur],sizeof(SIndex),1,f);
    cur++;
  }while((index[cur-1].id)&&(cur < MAXINDEX));


  //Now process these indexes one by one:
  //Call the appropriate Load functions when needed
  for (int indexnr=0 ; indexnr < cur; indexnr++){
    switch(index[indexnr].id){
    case(01): cout << "Found index for vertex information" << endl;
      fseek(f,index[indexnr].offset,SEEK_SET);
      if (!LoadVertices(f))
	return FALSE;
      break;
    case(02): cout << "Found index for faces information" << endl;
      fseek(f,index[indexnr].offset,SEEK_SET);
      if (!LoadFaces(f))
	return FALSE;
      break;
    case(03): cout << "Found index for object info " << endl;
      fseek(f,index[indexnr].offset,SEEK_SET);
      if (!LoadInfo(f))
	return FALSE;
      break;
    }
  }
  return TRUE;
}

/* This function loads vertex data
 * This is called from SObjectclass::Load
 * The file-marker is already positioned at the start of the vertex data.
 */
int SObjectclass::LoadVertices(FILE *f)
{
  //Load the Vertex data header structure:
  SStart info;
  fread(&info,sizeof(SStart),1,f);
  //Check if this is indeed vertex data:
  if (info.id != 01){
    cout << "NOT VERTICES!" << endl;
    return FALSE;
  }
  //Create room for the vertices in memory
  nrvertices = info.count;
  vertex = new SVertex[info.count];

  //Load the vertices from the file into memory
  fread(vertex,sizeof(SVertex),info.count,f);

  return TRUE;
}
/* This Load the Face data 
 * Called from SObjectclass::Load
 * The file is already positioned
 */
int SObjectclass::LoadFaces(FILE *f)
{
  //Load the faces data header:
  SStart info;
  fread(&info,sizeof(SStart),1,f);
  //Check is this is indeed faces data:
  if (info.id != 02){
    cout << "NOT FACES!" << endl;
    return FALSE;
  }
  //Make room for faces in memory
  nrfaces = info.count;
  face = new SFace[info.count];

  //Load faces data from file:
  fread(face,sizeof(SFace),info.count,f);  
  
  return TRUE;
}
/* This load the global object information
 * Called by SObjectclass::Load
 * File is already positioned
 */
int SObjectclass::LoadInfo(FILE *f)
{
  //Create room for Object information in memory
  info = new SObjInfo;
  //Load the data:
  fread(info,sizeof(SObjInfo),1,f);
  //check if this was indeed object information:
  if (info->id != 03){
    cout << "NOT OBJECT INFO!" << endl;
    return FALSE;
  }
  return TRUE;
}
//Call every objects Draw function
void SimpleObjectclass::Draw()
{
  for  (int i=0; i < nrobjects ; i++)
    objects[i].Draw();
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

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,1.33,0.1f,100.0f);   //setup the camera
   
  glClearColor(0.0,0.0,0.2,0.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-14);
  glRotatef(-60,1,0,0);
  glRotatef(-45,0,0,1);
  
  SimpleObjectclass object1;
  if (object1.Load("obj.simple")){
    cout << "Loaded object :)" << endl;
  }else{
    cout << "Failed to load object" << endl;
    return -1;
  }
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  object1.Draw();
  SDL_GL_SwapBuffers();
    
  cout << "Will quit after 3 seconds" << endl;
  SDL_Delay(3000);

  SDL_Quit();
  return 0;
}
