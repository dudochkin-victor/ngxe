//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit



#ifndef MATERIAL_HH
#define MATERIAL_HH

#include <GL/gl.h>


typedef class GLMaterialclass{
 public:
  GLMaterialclass();

  //global functions
  void EnableColorMaterial();
  void DisableColorMaterial()
    {glDisable(GL_COLOR_MATERIAL);}

  //setting properties 
  //note: diffuse and ambient are set using glColor
  void SetSpecular(const GLfloat *spec);
  void SetEmission(const GLfloat *emis);
  void SetShininess(const GLfloat shin)
    {shininess = shin;}

  //set the material properties active using opengl commands
  void SetActive(); 

 private:
  GLfloat specular[3];
  GLfloat emission[3];
  GLfloat shininess;

};

#endif


