//////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners2_extension.cpp
//	NV_register_combiners2 extension setup
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
#include "NV_register_combiners2_extension.h"

extern LOG errorLog;

bool NV_register_combiners2_supported;

bool SetUpNV_register_combiners2()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_NV_register_combiners2";

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
			NV_register_combiners2_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}


	if(!NV_register_combiners2_supported)
	{
		errorLog.OutputError("NV_register_combiners2 unsupported!");
		return false;
	}

	errorLog.OutputSuccess("NV_register_combiners2 supported!");

	//get function pointers
	glCombinerStageParameterfvNV		=	(PFNGLCOMBINERSTAGEPARAMETERFVNVPROC)
											wglGetProcAddress("glCombinerStageParameterfvNV");
	glGetCombinerStageParameterfvNV		=	(PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC)
											wglGetProcAddress("glGetCombinerStageParameterfvNV");

	return true;
}

PFNGLCOMBINERSTAGEPARAMETERFVNVPROC			glCombinerStageParameterfvNV			=NULL;
PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC		glGetCombinerStageParameterfvNV			=NULL;
