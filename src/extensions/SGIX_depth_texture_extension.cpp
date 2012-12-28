//////////////////////////////////////////////////////////////////////////////////////////
//	SGIX_depth_texture_extension.cpp
//	SGIX_depth_texture extension setup
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
#include "SGIX_depth_texture_extension.h"

extern LOG errorLog;

bool SGIX_depth_texture_supported=false;

bool SetUpSGIX_depth_texture()
{
	//Check for support
	char * extensionString=(char *)glGetString(GL_EXTENSIONS);
	char * extensionName="GL_SGIX_depth_texture";

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
			SGIX_depth_texture_supported=true;
		}

		//if not, move on
		extensionString+=distanceToSpace+1;
	}

	if(!SGIX_depth_texture_supported)
	{
		errorLog.OutputError("SGIX_depth_texture unsupported!");
		return false;
	}

	errorLog.OutputSuccess("SGIX_depth_texture supported!");

	//get function pointers
	//none specified

	return true;
}
