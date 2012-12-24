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
#include <GL/gl.h>

#include "lists.h"

#define TRUE 1
#define FALSE 0

GLListsclass::GLListsclass()
{
  lists = glGenLists(1);
  nrlists = 1;
  curlist = lists;
}
GLListsclass::GLListsclass(int nr)
{
  if (nr < 1) nr = 1;
  lists = glGenLists(nr);
  nrlists = nr;
  curlist = lists;
}
GLListsclass::~GLListsclass()
{
  //cleanup lists
  glDeleteLists(lists,nrlists);
}
int GLListsclass::Select(int nr)
{
  if ((nr >= 0)&&(nr < nrlists)){
    curlist = lists+nr;
    return TRUE;
  }else
    return FALSE;
}





