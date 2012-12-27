//////////////////////////////////////////////////////////////////////////////////////////
//	jpeg.h
//	Load a jpeg image
//	This is not part of the common code since it requires a jpeg library and headers
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef JPEG_H
#define JPEG_H

extern "C"
{
	#include "jpeglib.h"
}

bool LoadJPG(IMAGE * image, char * filename);

#endif	//JPEG_H