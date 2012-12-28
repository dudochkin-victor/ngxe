//////////////////////////////////////////////////////////////////////////////////////////
//	SGIX_shadow_extension.cpp
//	SGIX_shadow extension setup
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
#include "SGIX_shadow_extension.h"

extern LOG errorLog;

bool SGIX_shadow_supported=false;

bool SetUpSGIX_shadow()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_SGIX_shadow";

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
			SGIX_shadow_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}

	if(!SGIX_shadow_supported)
	{
		errorLog.OutputError("SGIX_shadow unsupported!");
		return false;
	}

	errorLog.OutputSuccess("SGIX_shadow supported!");

	//get function pointers
	//none specified

	return true;
}
