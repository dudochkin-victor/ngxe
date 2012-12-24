//This file is distributed under the GNU General Public License Copyright (C) 2005 Heinrich du Toit

#ifndef LIGHT_HH
#define LIGHT_HH

#include <GL/gl.h>

/*
 * This class provides basic light functionality
 *
 *
 */
typedef class GLLightclass{
 public:
  //constructors & selecting a light (GL_LIGHTi)
  GLLightclass();
  GLLightclass(GLenum l);
  void SetLight(GLenum l) {light = l;}

  //enabling and disabling lighting stuff
  void EnableLighting() {glEnable(GL_LIGHTING);}
  void DisableLighting() {glDisable(GL_LIGHTING);}
  void EnableLight() {glEnable(light);}
  void DisableLight() {glDisable(light);}

  //light model
  void LightModelAmbient(const GLfloat *color) 
    {glLightModelfv(GL_LIGHT_MODEL_AMBIENT,color);}

  //light properties
  void SetAmbient(const GLfloat *ambient)
    {glLightfv(light,GL_AMBIENT,ambient);}
  void SetDiffuse(const GLfloat *diffuse)
    {glLightfv(light,GL_DIFFUSE,diffuse);}
  void SetSpecular(const GLfloat *specular)
    {glLightfv(light,GL_SPECULAR,specular);}
  void SetPosition(const GLfloat *position)
    {glLightfv(light,GL_POSITION,position);}
  void SetSpotDirection(const GLfloat *spotdir)
    {glLightfv(light,GL_SPOT_DIRECTION,spotdir);}
  void SetSpotExponent(const GLfloat spotexp)
    {glLightf(light,GL_SPOT_EXPONENT,spotexp);}
  void SetSpotCutoff(const GLfloat spotcutoff)
    {glLightf(light,GL_SPOT_CUTOFF,spotcutoff);}


 private:
  GLenum light;

}GLLightclass;

/*
  TODO:
  add Get functions for completeness
  The attenuation functions!
  Create an inherited class with a movable light.
  Should be able to shine the spotlight @ some point.
 */


#endif

