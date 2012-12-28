//////////////////////////////////////////////////////////////////////////////////////////
//	ARB_texture_border_clamp_extension.cpp
//	ARB_texture_border_clamp extension setup
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
#include "ARB_texture_border_clamp_extension.h"

extern LOG errorLog;

bool ARB_texture_border_clamp_supported=false;

bool SetUpARB_texture_border_clamp()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_ARB_texture_border_clamp";

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
			ARB_texture_border_clamp_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}
	
	if(!ARB_texture_border_clamp_supported)
	{
		errorLog.OutputError("ARB_texture_border_clamp unsupported!");
		return false;
	}

	errorLog.OutputSuccess("ARB_texture_border_clamp supported!");

	//get function pointers
	//none specified

	return true;
}
