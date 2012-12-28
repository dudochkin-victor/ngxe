//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_multi_draw_arrays_extension.cpp
//	EXT_multi_draw_arrays extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <string.h>
#include <GL/gl.h>
#include <GL/glext.h>
//#include <GL\wglext.h>
#include "Util.h"
#include "EXT_multi_draw_arrays_extension.h"

bool EXT_multi_draw_arrays_supported=false;

bool SetUpEXT_multi_draw_arrays()
{
	if(!GL_EXT_multi_draw_arrays)
	{
		Util::log("EXT_multi_draw_arrays unsupported!");
		return false;
	}

	Util::log("EXT_multi_draw_arrays supported!");
	return true;
}
