//////////////////////////////////////////////////////////////////////////////////////////
//	ARB_texture_border_clamp_extension.cpp
//	ARB_texture_border_clamp extension setup
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
#include "ARB_texture_border_clamp_extension.h"

bool SetUpARB_texture_border_clamp() {

	if (!GL_ARB_texture_border_clamp) {
		Util::log("GL_ARB_texture_border_clamp unsupported!");
		return false;
	}

	Util::log("GL_ARB_texture_border_clamp supported!");
	return true;
}
