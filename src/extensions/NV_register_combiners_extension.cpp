//////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners_extension.cpp
//	NV_register_combiners extension setup
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
#include "NV_register_combiners_extension.h"

extern LOG errorLog;

bool NV_register_combiners_supported=false;

bool SetUpNV_register_combiners()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_NV_register_combiners";

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
			NV_register_combiners_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}

	if(!NV_register_combiners_supported)
	{
		errorLog.OutputError("NV_register_combiners unsupported!");
		return false;
	}

	errorLog.OutputSuccess("NV_register_combiners supported!");

	//get function pointers
	glCombinerParameterfvNV				=	(PFNGLCOMBINERPARAMETERFVNVPROC)
											wglGetProcAddress("glCombinerParameterfvNV");
	glCombinerParameterivNV				=	(PFNGLCOMBINERPARAMETERIVNVPROC)
											wglGetProcAddress("glCombinerParameterivNV");
	glCombinerParameterfNV				=	(PFNGLCOMBINERPARAMETERFNVPROC)
											wglGetProcAddress("glCombinerParameterfNV");
	glCombinerParameteriNV				=	(PFNGLCOMBINERPARAMETERINVPROC)
											wglGetProcAddress("glCombinerParameteriNV");

	glCombinerInputNV					=	(PFNGLCOMBINERINPUTNVPROC)
											wglGetProcAddress("glCombinerInputNV");
	glCombinerOutputNV					=	(PFNGLCOMBINEROUTPUTNVPROC)
											wglGetProcAddress("glCombinerOutputNV");

	glFinalCombinerInputNV				=	(PFNGLFINALCOMBINERINPUTNVPROC)
											wglGetProcAddress("glFinalCombinerInputNV");

	glGetCombinerInputParameterfvNV		=	(PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC)
											wglGetProcAddress("glGetCombinerInputParameterfvNV");
	glGetCombinerInputParameterivNV		=	(PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC)
											wglGetProcAddress("glGetCombinerInputParameterivNV");

	glGetCombinerOutputParameterfvNV	=	(PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC)
											wglGetProcAddress("glGetCombinerOutputParameterfvNV");
	glGetCombinerOutputParameterivNV	=	(PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC)
											wglGetProcAddress("glGetCombinerOutputParameterivNV");

	glGetFinalCombinerInputParameterfvNV=	(PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC)
											wglGetProcAddress("glGetFinalCombinerInputParameterfvNV");
	glGetFinalCombinerInputParameterivNV=	(PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC)
											wglGetProcAddress("glGetFinalCombinerInputParameterivNV");

	return true;
}


PFNGLCOMBINERPARAMETERFVNVPROC				glCombinerParameterfvNV					=NULL;
PFNGLCOMBINERPARAMETERIVNVPROC				glCombinerParameterivNV					=NULL;
PFNGLCOMBINERPARAMETERFNVPROC				glCombinerParameterfNV					=NULL;
PFNGLCOMBINERPARAMETERINVPROC				glCombinerParameteriNV					=NULL;

PFNGLCOMBINERINPUTNVPROC					glCombinerInputNV						=NULL;
PFNGLCOMBINEROUTPUTNVPROC					glCombinerOutputNV						=NULL;

PFNGLFINALCOMBINERINPUTNVPROC				glFinalCombinerInputNV					=NULL;

PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC		glGetCombinerInputParameterfvNV			=NULL;
PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC		glGetCombinerInputParameterivNV			=NULL;

PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC		glGetCombinerOutputParameterfvNV		=NULL;
PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC		glGetCombinerOutputParameterivNV		=NULL;

PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC	glGetFinalCombinerInputParameterfvNV	=NULL;
PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC	glGetFinalCombinerInputParameterivNV	=NULL;
