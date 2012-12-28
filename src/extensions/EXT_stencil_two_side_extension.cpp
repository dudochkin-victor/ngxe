//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_stencil_two_side_extension.cpp
//	EXT_stencil_two_side extension setup
//	Downloaded from: www.paulsprojects.net
//	Created:	30th September 2002
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
#include "EXT_stencil_two_side_extension.h"

extern LOG errorLog;

bool EXT_stencil_two_side_supported=false;

bool SetUpEXT_stencil_two_side()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_EXT_stencil_two_side";

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
			EXT_stencil_two_side_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}

	if(!EXT_stencil_two_side_supported)
	{
		errorLog.OutputError("EXT_stencil_two_side unsupported!");
		return false;
	}

	errorLog.OutputSuccess("EXT_stencil_two_side supported!");

	//get function pointers
	glActiveStencilFaceEXT			=	(PFNGLACTIVESTENCILFACEEXTPROC)
										wglGetProcAddress("glActiveStencilFaceEXT");

	return true;
}

//function pointers
PFNGLACTIVESTENCILFACEEXTPROC				glActiveStencilFaceEXT			=NULL;


