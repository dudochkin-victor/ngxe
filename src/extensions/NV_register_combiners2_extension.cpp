//////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners2_extension.cpp
//	NV_register_combiners2 extension setup
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
#include "NV_register_combiners2_extension.h"

bool SetUpNV_register_combiners2() {

	if (!GL_NV_register_combiners2) {
		Util::log("GL_NV_register_combiners2 unsupported!");
		return false;
	}

	Util::log("GL_NV_register_combiners2 supported!");
	return true;
}
