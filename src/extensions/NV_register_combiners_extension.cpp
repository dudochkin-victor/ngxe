//////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners_extension.cpp
//	NV_register_combiners extension setup
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
#include "NV_register_combiners_extension.h"

bool SetUpNV_register_combiners() {

	if (!GL_NV_register_combiners) {
		Util::log("GL_NV_register_combiners unsupported!");
		return false;
	}

	Util::log("GL_NV_register_combiners supported!");
	return true;
}
