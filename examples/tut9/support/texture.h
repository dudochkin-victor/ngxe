#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <GL/gl.h>

typedef class Textureclass{
 public:
  Textureclass();
  ~Textureclass();

  //call before Load (NOT AFTER!)
  void Mipmap(); 
  void NoMipmap(); //default;
  void Compress();
  void NoCompress(); //default;

  int Load(const char* filename);

  void Activate() {glBindTexture(GL_TEXTURE_2D,texture);}

  //other options

  void SetWrap(GLint mode);

  //call after Load to setup the filter settings.
  void SetLinearFilter();
  void SetNearestFilter();
  
 protected:
  GLuint texture;
  int mipmap,compress;

}Textureclass;



#endif
