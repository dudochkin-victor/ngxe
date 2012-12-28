//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture3D_extension.cpp
//	EXT_texture3D extension setup
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
#include "EXT_texture3D_extension.h"

bool SetUpEXT_texture3D() {

	if (!GL_EXT_texture3D) {
		Util::log("GL_EXT_texture3D unsupported!");
		return false;
	}

	Util::log("GL_EXT_texture3D supported!");
	return true;
}

