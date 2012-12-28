//////////////////////////////////////////////////////////////////////////////////////////
//	WGL_ARB_extensions_string_extension.cpp
//	WGL_ARB_extensions_string extension setup
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
#include "WGL_ARB_extensions_string_extension.h"

bool SetUpWGL_ARB_extensions_string()
{
	
//	if (!wglGetExtensionsStringARB) {
		Util::log("wglGetExtensionsStringARB unsupported!");
		return false;
//	}

//	Util::log("wglGetExtensionsStringARB supported!");
//	return true;
}
