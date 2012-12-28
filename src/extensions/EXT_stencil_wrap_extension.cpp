//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_stencil_wrap_extension.cpp
//	EXT_stencil_wrap extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	30th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "Util.h"
#include "EXT_stencil_wrap_extension.h"

bool SetUpEXT_stencil_wrap() {

	if (!GL_EXT_stencil_wrap) {
		Util::log("GL_EXT_stencil_wrap unsupported!");
		return false;
	}

	Util::log("GL_EXT_stencil_wrap supported!");
	return true;
}

