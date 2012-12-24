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

#include "video.h"
#include "error.h"

int main(){
  cout << "Example error1 running. "<< endl;

  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    cout << "Error initializing SDL" << endl;
    return -1;
  }
  //Step 2 is to get a screen/window on which we can do some graphics.

  //create a Videoclass to control the video with
  SDLVideoclass SDLmain(320,200);
  
  if (SDLmain.InitVideo()){
    cout << "Video up and running" << endl;
  }else{
    cout << "Error initializing Video" << endl;
    return 0;
  }
  
  
  glBegin(GL_TRIANGLES);
  glGenLists(3); //may not execute this between Begin/End! GL_INVALID_OPERATION
  glEnd();
  
  //the long way of checking for gl errors:
  if (GLError::Check()){
    cout << "Got error:" << GLError::Name() << endl;
  }else
    cout << "No error" << endl;

  //the fast way:
  //or alternatively just run say:
  // CHECKGLSILENT
  /*
    To run the fast way:
    1. comment out the whole if statement for Check()
    2. uncomment the CHECKGLSILENT statement
   */

  cout<< "Byebye" << endl;

  SDL_Quit();
  return 0;
}
