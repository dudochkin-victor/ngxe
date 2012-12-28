//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture_env_combine_extension.cpp
//	EXT_texture_env_combine extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <string.h>
#include <string>
#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

//#include <GL\wglext.h>
#include "Util.h"
#include "EXT_texture_env_combine_extension.h"

bool EXT_texture_env_combine_supported=false;

bool SetUpEXT_texture_env_combine()
{
	//Check for support
	if(!GL_EXT_texture_env_combine)
	{
		Util::log("EXT_texture_env_combine unsupported!");
		return false;
	}

	Util::log("EXT_texture_env_combine supported!");

	//get function pointers
	//none specified

	return true;
}
