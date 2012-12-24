/*
 SDL Keyboard example - Tutorial 2

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

// We want some basic output capabilities for debugging.
#include <iostream>
using namespace std; //Dont forget this ;)

//include the basic header files for SDL and OpenGL
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
//please see the make file for compilation instructions
#define TRUE 1
#define FALSE 0


/*
 This class gives basic methods to use with SDL event structure.
 
 You can use this class with inheritance and increase the functionality
 or overwrite functionality :)
 */
typedef class SDLEventclass{
public:
  SDLEventclass();
  //this function handles incoming events.
  int ProcessEvent(); //returns true if there was an event to process

  SDL_Event *GetLastEvent(){return &lastevent;}
  
  Uint8 GetKey(int keysym) {return keystate[keysym];}
  Uint8 GetKeySafe(int keysym); //same as GetKey but make a safety check on the number of keys
  int CheckKeySym(int keysym);

  int MustQuit(){return quitrequested;}
protected:
  int numkeys;
  Uint8 *keystate;
  SDL_Event lastevent; 
private:
  int quitrequested; //did we recieve a quit event?

}SDLEventclass;

//The constructor
SDLEventclass::SDLEventclass()
{
  quitrequested = FALSE;
  keystate = SDL_GetKeyState(&numkeys);
  //debug:
  cout << "Got keystate of " << numkeys << " keys" << endl;
}

int SDLEventclass::ProcessEvent()
{
  SDL_keysym *keysym;
  if (SDL_PollEvent(&lastevent)){
    //so there is an event:
    //lets process it!
    switch (lastevent.type){
    case (SDL_QUIT): //this is a very important event
      quitrequested = TRUE;
      cout << "Recieved quit event" << endl;
      break;
    case (SDL_KEYDOWN): // a key was pressed down
      keysym = &(lastevent.key.keysym);
      cout << "Key pressed: " << SDL_GetKeyName(keysym->sym);
      cout << " Scancode: " << (int)keysym->scancode << endl; 
      if (keysym->sym == SDLK_ESCAPE)
	quitrequested = TRUE;
      break;
    case (SDL_KEYUP):
      keysym = &(lastevent.key.keysym);
      cout << "Key released: " << SDL_GetKeyName(keysym->sym);
      cout << " Scancode: " << (int)keysym->scancode << endl; 
      break;
    }

    return TRUE;
  }else
    return FALSE;
}
Uint8 SDLEventclass::GetKeySafe(int keysym)
{
  if ((keysym < numkeys)&&(keysym >= 0)){
    return keystate[keysym];
  }else
    return 0;
}
int SDLEventclass::CheckKeySym(int keysym)
{
  if ((keysym < numkeys)&&(keysym >= 0))
    return TRUE;
  else
    return FALSE;
}

int main()
{
  cout << "This is Tut2-keys example running" << endl;

  //Step 1 is to initialize SDL. Absolutely critical that your SDL application starts with this!
  if (SDL_Init(SDL_INIT_VIDEO) != 0){ 
    cout << "Error initializing SDL" << endl;
    return -1;
  }
  //we're not really intrested in video but the events of the keyboard must come throught the video interface.
  if (SDL_SetVideoMode(320,200,16,0)== NULL){
    cout << "Error create video window" << endl;
    return -1;
  }

  //Step 2 
  //Create a event class
  SDLEventclass events; //note: created static class (no pointers)

  cout << "Press ESC to quit example / Or press the close window for the small video screen" << endl;
  cout << "The up arrow is monitored constantly" << endl;
  while (!events.MustQuit()){ //was a quit requested?
    events.ProcessEvent(); //process new events
    SDL_Delay(25);  //just to slowdown things
    if (events.GetKey(SDLK_UP)){
      cout << "Up arrow is pressed down" << endl;
    }
  }

  cout << "Leaving..." << endl;
  SDL_Quit();
  return 0;
}
