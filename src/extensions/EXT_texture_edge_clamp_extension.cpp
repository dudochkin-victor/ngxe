//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture_edge_clamp_extension.cpp
//	EXT_texture_edge_clamp extension setup
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
#include "Util.h"
#include "EXT_texture_edge_clamp_extension.h"

bool SetUpEXT_texture_edge_clamp() {

	if (!GL_EXT_texture_edge_clamp) {
		Util::log("GL_EXT_texture_edge_clamp unsupported!");
		return false;
	}

	Util::log("GL_EXT_texture_edge_clamp supported!");
	return true;
}
