//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_render_texture_extension.cpp
//	WGL_ARB_render_texture extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	9th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "Util.h"
#include "WGL_ARB_render_texture_extension.h"

bool SetUpWGL_ARB_render_texture(const char * wglExtensions) {

//	if (!WGL_ARB_render_texture) {
		Util::log("WGL_ARB_render_texture unsupported!");
		return false;
//	}

//	Util::log("WGL_ARB_render_texture supported!");
//	return true;
}
