//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit



#ifndef EVENT_HH
#define EVENT_HH

#include <SDL/SDL.h>

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

  SDL_Event *GetLastEvent(){return &curevent;}
  
  Uint8 GetKey(int keysym) {return keystate[keysym];}
  Uint8 GetKeySafe(int keysym); //same as GetKey but make a safety check on the number of keys
  int CheckKeySym(int keysym);

  int MustQuit(){return quitrequested;}
protected:
  int numkeys;
  Uint8 *keystate;
  SDL_Event curevent; 
private:
  int quitrequested; //did we recieve a quit event?

}SDLEventclass;


#endif
