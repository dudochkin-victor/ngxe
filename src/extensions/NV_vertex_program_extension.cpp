//////////////////////////////////////////////////////////////////////////////////////////
//	NV_vertex_program_extension.cpp
//	NV_vertex_program extension setup
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
#include "NV_vertex_program_extension.h"

bool SetUpNV_vertex_program() {

	if (!GL_NV_vertex_program) {
		Util::log("GL_NV_vertex_program unsupported!");
		return false;
	}

	Util::log("GL_NV_vertex_program supported!");
	return true;
}
