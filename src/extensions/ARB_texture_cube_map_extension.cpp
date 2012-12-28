//////////////////////////////////////////////////////////////////////////////////////////
//	ARB_texture_cube_map_extension.cpp
//	ARB_texture_cube_map extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	26th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <Util.h>
#include "ARB_texture_cube_map_extension.h"

bool SetUpARB_texture_cube_map() {

	if (!GL_ARB_texture_cube_map) {
		Util::log("GL_ARB_texture_cube_map unsupported!");
		return false;
	}

	Util::log("GL_ARB_texture_cube_map supported!");
	return true;
}
