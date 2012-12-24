//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit

#ifndef TEXTURE_HH
#define TEXTURE_HH
#include <GL/gl.h>


/*
 * A very basic texture handling class
 */
typedef class GLTextureclass{
 public:
  GLTextureclass();
  ~GLTextureclass();

  int Load(const char* filename);

  void Activate() {glBindTexture(GL_TEXTURE_2D,texture);}

  //other options

  void SetWrap(GLint mode);
  void SetLinearFilter();
  
 protected:
  GLuint texture;


}GLTextureclass;



#endif
