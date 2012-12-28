//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_draw_range_elements_extension.cpp
//	EXT_draw_range_elements extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <string.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <Util.h>

#include "EXT_draw_range_elements_extension.h"

bool EXT_draw_range_elements_supported = false;

bool SetUpEXT_draw_range_elements() {
	//Check for support
	if (!GL_EXT_draw_range_elements) {
		Util::log("EXT_draw_range_elements unsupported!");
		return false;
	}
	Util::log("EXT_draw_range_elements supported!");

	return true;
}

