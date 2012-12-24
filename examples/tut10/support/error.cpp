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
#include <GL/gl.h>

#include "error.h"


const char* noerror = "No Error";
const char* unknown = "Unkown Error";
const char* invenum = "Unacceptable enumerated value Specified";
const char* invval = "A numarical argument is out of range";
const char* invoper = "The instruction is not allowed in the current state";
const char* stackoverflow = "The command will cause a stack overflow";
const char* stackunderflow = "The command will cause a stack underflow";
const char* outofmem = "Not enough memory available to execute command";
const char* tablelarge = "The specified table is too large";

GLenum GLError::lasterror;

int GLError::Check()
{
  lasterror = glGetError();
  return (lasterror != GL_NO_ERROR);
}

const char* GLError::Name()
{
  switch (lasterror){
  case(GL_NO_ERROR): 
    return noerror;
  case(GL_INVALID_ENUM):
    return invenum;
  case(GL_INVALID_VALUE):
    return invval;
  case(GL_INVALID_OPERATION):
    return invoper;
  case(GL_STACK_OVERFLOW):
    return stackoverflow;
  case(GL_STACK_UNDERFLOW):
    return stackunderflow;
  case(GL_OUT_OF_MEMORY):
    return outofmem;
  case(GL_TABLE_TOO_LARGE):
    return tablelarge;
  default:
    return unknown;
  }
}


void GLError::Silent(char* filename,int linenumber)
{
  lasterror = glGetError();
  if (lasterror != GL_NO_ERROR){
    std::cout << filename << ":" << linenumber << " GL ERROR: " << Name() << std::endl;
  }
}
