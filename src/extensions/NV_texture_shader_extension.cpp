//////////////////////////////////////////////////////////////////////////////////////////
//	NV_texture_shader_extension.cpp
//	NV_texture_shader extension setup
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
#include "NV_texture_shader_extension.h"

bool SetUpNV_texture_shader() {

	if (!GL_NV_texture_shader) {
		Util::log("GL_NV_texture_shader unsupported!");
		return false;
	}

	Util::log("GL_NV_texture_shader supported!");
	return true;
}
