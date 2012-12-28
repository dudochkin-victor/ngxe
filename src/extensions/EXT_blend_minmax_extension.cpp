//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_blend_minmax_extension.cpp
//	EXT_blend_minmax extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "Util.h"
#include "EXT_blend_minmax_extension.h"

bool SetUpEXT_blend_minmax() {

	if (!GL_EXT_blend_minmax) {
		Util::log("GL_EXT_blend_minmax unsupported!");
		return false;
	}

	Util::log("GL_EXT_blend_minmax supported!");
	return true;
}

