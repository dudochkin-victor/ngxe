//////////////////////////////////////////////////////////////////////////////////////////
//	SGIX_shadow_extension.cpp
//	SGIX_shadow extension setup
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
#include "SGIX_shadow_extension.h"

bool SetUpSGIX_shadow() {

	if (!GL_SGIX_shadow) {
		Util::log("GL_SGIX_shadow unsupported!");
		return false;
	}

	Util::log("GL_SGIX_shadow supported!");
	return true;
}
