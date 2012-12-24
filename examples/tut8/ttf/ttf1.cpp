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
 * Example on displaying True Type fonts in opengl using freetype library
 *  Tutorial 8
 *
 */
#include <iostream>
using namespace std;

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include <video.h>
#include <event.h>
#include <error.h>

//include freetype header file
#include <ft2build.h>
#include FT_FREETYPE_H

#define TRUE 1
#define FALSE 0

typedef class Textclass{
public:
  Textclass();
  int Load(char * filename,int size);
  void PutChar(char c,GLfloat x,GLfloat y,GLfloat w,GLfloat h);
  void PutString(char *text,GLfloat x,GLfloat y,GLfloat size);
  int GetCharWidth(char c){return width[int(c)];} //if 0 then char not in set!
  int GetStringWidth(char *text);
  int GetCharSize(){return charsize;}

private:
  GLuint fonttexture;
  int charsize;
  int texsize;
  GLfloat *coords;
  char width[256];
  char height[256];
  

}Textclass;
Textclass::Textclass()
{
  fonttexture = 0;
  charsize = 0;
  coords = NULL;
  memset(width,0,sizeof(char)*256);
  memset(height,0,sizeof(char)*256);
}
int Textclass::Load(char * filename,int size)
{
  FT_Library ftlibrary;
  FT_Face ftface;
  charsize = size;
  //open freetype library
  int error;
  if (FT_Init_FreeType(&ftlibrary)){
    cout << "Error initializing freetype library" << endl;
    return FALSE;
  }
  //load font:
  error = FT_New_Face(ftlibrary,filename,0,&ftface);
  if (error){
    cout << "Error loading font" << endl;
    return FALSE;
  }
  //set pixel size for the font.
  error = FT_Set_Pixel_Sizes(ftface,0,size);
  if (error){
    cout << "Error setting pixel size" << endl;
    return FALSE;
  }
  //determine texture size;
  int texw,texh;
  texw = (size*16);
  if (texw > 1024){
    cout << "Text to large" << endl;
    return FALSE;
  }else if (texw > 512){
    texw = 1024;
  }else if (texw > 256){
    texw = 512;
  }else if (texw > 128){
    texw = 256;
  }else{
    texw = 128;
  }
  cout << texw << endl;
  texh = texw;
  texsize = texw;
  //create a temporary bitmap for the texture.
  unsigned char *tmpbitmap = new unsigned char[2*texw*texh];
  memset(tmpbitmap,0,2*texw*texh);

  //create the array to hold the coordinates
  coords = new GLfloat[256*4];
  memset(coords,0,sizeof(GLfloat)*256*4);
  memset(width,0,sizeof(char)*256);
  memset(height,0,sizeof(char)*256);
  
  int x=1;
  int y=1;
  int ymax=0;
  
  //Load the characters and copy them to the bitmap:
  
  for (int i = 0; i < 256; i++){
    //check if char exist?
    error = FT_Get_Char_Index(ftface,i);
    if (!error) continue;  //don't want all those cube thingies!
    //load char?
    error =  FT_Load_Char(ftface,(i),FT_LOAD_RENDER);
    if (error) continue;
    //find the bitmap
    FT_Bitmap *bitmap = &(ftface->glyph->bitmap);

    if (x+bitmap->width+1 >= texw){ //new line in bitmap?
      x = 0;
      y += ymax+2;
      if (y + 16 > texh){
	cout << "problematic error :(" << endl;
	return FALSE;
      }
      ymax = 0;
    }
    //calculate coordinates for the char
    coords[(i)*4+0] = GLfloat(x-1)/texw;
    coords[(i)*4+1] = GLfloat(y+bitmap->rows+1)/texh;
    coords[(i)*4+2] = GLfloat(x+bitmap->width+1)/texw;
    coords[(i)*4+3] = GLfloat(y-1)/texh;
    width[i] = bitmap->width;
    height[i] = bitmap->rows;
    //put text in bitmap at location x,y
    for (int k=0; k < bitmap->width;k++)
      for (int l=0; l < bitmap->rows; l++){
	tmpbitmap[((l+y)*texw+(k+x))*2] = bitmap->buffer[l*bitmap->width+k];
	tmpbitmap[((l+y)*texw+(k+x))*2+1] = bitmap->buffer[l*bitmap->width+k];
      }
    //check height (for next row)
    if (bitmap->rows > ymax) 
      ymax = bitmap->rows;
    //goto next x position
    x += bitmap->width+2;
  }
  //  y += ymax+2;

  glGenTextures(1,&fonttexture);
  glBindTexture(GL_TEXTURE_2D,fonttexture);

  glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,texw,texh,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,tmpbitmap);

  delete [] tmpbitmap;
  //Check for errors:
  if (glGetError() != GL_NO_ERROR){
    cout << "glTexImage2D failed"<< endl;
    return FALSE;
  }
  //nice filtering :)
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  FT_Done_Face(ftface);
  FT_Done_FreeType(ftlibrary);

  return TRUE;
}
void Textclass::PutChar(char c,GLfloat x,GLfloat y,GLfloat w,GLfloat h)
{
  GLfloat x1,x2;
  GLfloat y1,y2;
  x1 = coords[c*4+0];
  y1 = coords[c*4+1];
  x2 = coords[c*4+2];
  y2 = coords[c*4+3];
  w = w*width[int(c)]/charsize;
  h = h*height[int(c)]/charsize;
  glBindTexture(GL_TEXTURE_2D,fonttexture);
  glBegin(GL_QUADS);
  glTexCoord2f(x1,y1);
  glVertex2f(x,y);
  glTexCoord2f(x2,y1);
  glVertex2f(x+w,y);
  glTexCoord2f(x2,y2);
  glVertex2f(x+w,y+h);
  glTexCoord2f(x1,y2);
  glVertex2f(x,y+h);
  glEnd();
}
void Textclass::PutString(char *text,GLfloat x,GLfloat y,GLfloat size)
{
  if (charsize == 0) return;
  while (*text){
    PutChar(*text,x,y,size,size);
    x += size*1.1*width[int(*text)]/charsize;
    text++;
  }
}
int Textclass::GetStringWidth(char *text)
{
  int answer = 0;
  while (*text){
    answer += width[int(*text)];
    text++;
  }
  return answer;
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

  //  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glShadeModel(GL_SMOOTH); //use smooth shading mode.

  //setup camera:
  glMatrixMode(GL_PROJECTION);

  //setup 2D ortho viewing 
  gluOrtho2D(-1.3,1.3,-1,1);
   
  glClearColor(0.0,0.0,0.2,0.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //MODELVIEW matrix need to be Identity because we use ortho view

  //  Setup();
  Textclass text;
  text.Load("comic.ttf",16);

  glEnable(GL_TEXTURE_2D);


  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1,1,1);
  
  //this will display the complete texture with the characters on it
  glBegin(GL_QUADS);
  glTexCoord2i(1,0);
  glVertex2f(1,1);
  glTexCoord2i(1,1);
  glVertex2f(1,0);
  glTexCoord2i(0,1);
  glVertex2f(0,0);
  glTexCoord2i(0,0);
  glVertex2f(0,1);
  glEnd();

  
  //enable blending: 
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


  text.PutString("Hello World! :) 1234",-1,-0.3,0.2);

  glColor3f(1,0.5,0.5); //tint charachters slightly red
  text.PutChar('8',-1,-1,0.3,0.3);
  text.PutChar('a',-0.7,-1,0.3,0.3);
  text.PutChar('h',-0.4,-1,0.3,0.3);
  text.PutChar('*',-0.1,-1,0.3,0.3);

  SDL_GL_SwapBuffers();

  SDL_Delay(3000);

  SDL_Quit();
  return 0;
}

