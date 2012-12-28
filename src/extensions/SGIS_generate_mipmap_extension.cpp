//////////////////////////////////////////////////////////////////////////////////////////
//	SGIS_generate_mipmap_extension.cpp
//	SGIS_generate_mipmap extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	10th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "Util.h"
#include "SGIS_generate_mipmap_extension.h"

bool SetUpSGIS_generate_mipmap() {

	if (!GL_SGIS_generate_mipmap) {
		Util::log("GL_SGIS_generate_mipmap unsupported!");
		return false;
	}

	Util::log("GL_SGIS_generate_mipmap supported!");
	return true;
}
