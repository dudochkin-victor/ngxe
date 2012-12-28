//////////////////////////////////////////////////////////////////////////////////////////
//	ARB_multitexture_extension.cpp
//	ARB_multitexture extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include "../LOG.h"
#include "ARB_multitexture_extension.h"

extern LOG errorLog;

bool ARB_multitexture_supported=false;

bool SetUpARB_multitexture()
{
	if(!GL_ARB_multitexture)
	{
		errorLog.OutputError("ARB_multitexture unsupported!");
		return false;
	}

	errorLog.OutputSuccess("ARB_multitexture supported!");
	return true;
}


