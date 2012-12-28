//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture_filter_anisotropic_extension.cpp
//	EXT_texture_filter_anisotropic extension setup
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
#include "EXT_texture_filter_anisotropic_extension.h"

bool SetUpEXT_texture_filter_anisotropic() {

	if (!GL_EXT_texture_filter_anisotropic) {
		Util::log("GL_EXT_texture_filter_anisotropic unsupported!");
		return false;
	}

	Util::log("GL_EXT_texture_filter_anisotropic supported!");
	return true;
}
