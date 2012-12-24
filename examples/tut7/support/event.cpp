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

#include <SDL/SDL.h>

#include "event.h"

#define TRUE 1
#define FALSE 0

//The constructor
SDLEventclass::SDLEventclass()
{
  quitrequested = FALSE;
  keystate = SDL_GetKeyState(&numkeys);
}

int SDLEventclass::ProcessEvent()
{
  SDL_keysym *keysym;
  if (SDL_PollEvent(&curevent)){
    //so there is an event:
    //lets process it!
    switch (curevent.type){
    case (SDL_QUIT): 
      quitrequested = TRUE;
      break;
    case (SDL_KEYDOWN): 
      keysym = &(curevent.key.keysym);
      if (keysym->sym == SDLK_ESCAPE)
	quitrequested = TRUE;
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

