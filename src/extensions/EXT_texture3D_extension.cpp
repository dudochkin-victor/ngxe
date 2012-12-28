//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture3D_extension.cpp
//	EXT_texture3D extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include <GL\glext.h>
#include <GL\wglext.h>
#include "../LOG.h"
#include "EXT_texture3D_extension.h"

extern LOG errorLog;

bool EXT_texture3D_supported=false;

bool SetUpEXT_texture3D()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_EXT_texture3D";

	char * endOfString;									//store pointer to end of string
	unsigned int distanceToSpace;						//distance to next space

	endOfString=extensionString+strlen(extensionString);

	//loop through string
	while(extensionString<endOfString)
	{
		//find distance to next space
		distanceToSpace=strcspn(extensionString, " ");

		//see if we have found extensionName
		if((strlen(extensionName)==distanceToSpace) &&
			(strncmp(extensionName, extensionString, distanceToSpace)==0))
		{
			EXT_texture3D_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}

	if(!EXT_texture3D_supported)
	{
		errorLog.OutputError("EXT_texture3D unsupported!");
		return false;
	}

	errorLog.OutputSuccess("EXT_texture3D supported!");

	//get function pointers
	glTexImage3DEXT						=	(PFNGLTEXIMAGE3DEXTPROC)
											wglGetProcAddress("glTexImage3DEXT");

	return true;
}

PFNGLTEXIMAGE3DEXTPROC						glTexImage3DEXT					=NULL;
