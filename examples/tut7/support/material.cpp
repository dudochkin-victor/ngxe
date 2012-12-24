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
#include <GL/gl.h>
#include <GL/glu.h>

#include "material.h"

#define TRUE 1
#define FALSE 0

GLMaterialclass::GLMaterialclass()
{
  emission[0] = 0.0;
  emission[1] = 0.0;
  emission[2] = 0.0;
  specular[0] = 0.0;
  specular[1] = 0.0;
  specular[2] = 0.0;
  shininess = 0;
}
void GLMaterialclass::EnableColorMaterial()
{
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}
void GLMaterialclass::SetSpecular(const GLfloat *spec)
{
  specular[0] = spec[0];
  specular[1] = spec[1];
  specular[2] = spec[2];
}
void GLMaterialclass::SetEmission(const GLfloat *emis)
{
  emission[0] = emis[0];
  emission[1] = emis[1];
  emission[2] = emis[2];
}
void GLMaterialclass::SetActive()
{
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,emission);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
}
