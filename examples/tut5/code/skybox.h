//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit

#ifndef SKYBOX_HH
#define SKYBOX_HH


#include "lists.h"
#include "texture.h"



// the sky box class:
typedef class SkyBox{
 public:
  //Load and create skybox
  int Load(char* texfilename);

  //Draw the skybox
  void Draw() {dlist.Execute();}

 private:
  void DrawBox();
  GLTextureclass texture;
  GLListsclass dlist;
  
}SkyBox;









#endif
