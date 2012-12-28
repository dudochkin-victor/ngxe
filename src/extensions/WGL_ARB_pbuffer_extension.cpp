//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_pbuffer_extension.cpp
//	WGL_ARB_pbuffer extension setup
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
#include "WGL_ARB_pbuffer_extension.h"

bool SetUpWGL_ARB_pbuffer(const char * wglExtensions) {

//	if (!WGL_ARB_pbuffer) {
		Util::log("WGL_ARB_pbuffer unsupported!");
//		return false;
//	}

//	Util::log("WGL_ARB_pbuffer supported!");
	//sreturn true;
}

