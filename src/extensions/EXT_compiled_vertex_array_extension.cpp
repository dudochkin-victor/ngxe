//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_compiled_vertex_array_extension.cpp
//	EXT_compiled_vertex_array extension setup
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
#include "EXT_compiled_vertex_array_extension.h"

bool SetUpEXT_compiled_vertex_array() {

	if (!GL_EXT_compiled_vertex_array) {
		Util::log("GL_EXT_compiled_vertex_array unsupported!");
		return false;
	}

	Util::log("GL_EXT_compiled_vertex_array supported!");
	return true;
}

