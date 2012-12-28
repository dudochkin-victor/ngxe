//////////////////////////////////////////////////////////////////////////////////////////
//	SGIX_depth_texture_extension.cpp
//	SGIX_depth_texture extension setup
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
#include "SGIX_depth_texture_extension.h"

bool SetUpSGIX_depth_texture() {

	if (!GL_SGIX_depth_texture) {
		Util::log("GL_SGIX_depth_texture unsupported!");
		return false;
	}

	Util::log("GL_SGIX_depth_texture supported!");
	return true;
}
