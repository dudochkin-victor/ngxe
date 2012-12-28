//////////////////////////////////////////////////////////////////////////////////////////
//	PBUFFER.cpp
//	Functions to set up a pBuffer
//	Downloaded from: www.paulsprojects.net
//	Created:	9th September 2002
//	Updated:	20th August 2003	-	Init function cleaned up, caller must now specify all
//										attributes and flags
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <windows.h>
#include "GL files/glee.h"
#include <GL/glu.h>
#include "Log/LOG.h"
#include "PBUFFER.h"

bool PBUFFER::Init(	int newWidth, int newHeight,
					const int * attribIList, const float * attribFList, const int * flags)
{
	//Check for pbuffer support
	if(	!GLEE_WGL_ARB_extensions_string	||
		!GLEE_WGL_ARB_pixel_format		||
		!GLEE_WGL_ARB_pbuffer)
	{
		LOG::Instance()->OutputError("Extension required for pBuffer unsupported");
		return false;
	}

	//set class's member variables
	width=newWidth;
	height=newHeight;
	
	//Get the current device context
	HDC hCurrentDC=wglGetCurrentDC();
	if(!hCurrentDC)
	{
		LOG::Instance()->OutputError("Unable to get current Device Context");
		return false;
	}
	
	
	//choose pixel format
	GLint pixelFormat;
	unsigned int numFormats;

	if(!wglChoosePixelFormatARB(hCurrentDC, attribIList, attribFList, 1,
								&pixelFormat, &numFormats))
	{
		LOG::Instance()->OutputError("Unable to find a pixel format for the pbuffer");
		return false;
	}

	//Create the pbuffer
	hBuffer=wglCreatePbufferARB(hCurrentDC, pixelFormat, width, height, flags);
	if(!hBuffer)
	{
		LOG::Instance()->OutputError("Unable to create pbuffer");
		return false;
	}

	//Get the pbuffer's device context
	hDC=wglGetPbufferDCARB(hBuffer);
	if(!hDC)
	{
		LOG::Instance()->OutputError("Unable to get pbuffer's device context");
		return false;
	}

	//Create a rendering context for the pbuffer
	hRC=wglCreateContext(hDC);
	if(!hRC)
	{
		LOG::Instance()->OutputError("Unable to create pbuffer's rendering context");
		return false;
	}

	//Set and output the actual pBuffer dimensions
	wglQueryPbufferARB(hBuffer, WGL_PBUFFER_WIDTH_ARB, &width);
	wglQueryPbufferARB(hBuffer, WGL_PBUFFER_HEIGHT_ARB, &height);
	LOG::Instance()->OutputSuccess("Pbuffer Created: (%d x %d)", width, height);
	
	return TRUE;										//success!
}

void PBUFFER::Shutdown(void)
{
	if(hRC)							//have a rendering context?
	{
		if(!wglDeleteContext(hRC))	//try to delete RC
		{
			LOG::Instance()->OutputError("Release of Pbuffer Rendering Context Failed.");
		}
		
		hRC=NULL;
	}

	if(hDC && !wglReleasePbufferDCARB(hBuffer, hDC))	//Are we able to release DC?
	{
		LOG::Instance()->OutputError("Release of Pbuffer Device Context Failed.");
		hDC=NULL;
	}
	
	if(!wglDestroyPbufferARB(hBuffer))
	{
		LOG::Instance()->OutputError("Unable to destroy pbuffer");
	}
}


bool PBUFFER::MakeCurrent()
{
	if(!wglMakeCurrent(hDC, hRC))
	{
		LOG::Instance()->OutputError("Unable to change current context");
		return false;
	}

	return true;
}

