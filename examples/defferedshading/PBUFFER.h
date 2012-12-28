//////////////////////////////////////////////////////////////////////////////////////////
//	PBUFFER.h
//	Class to setup pBuffer
//	Downloaded from: www.paulsprojects.net
//	Created:	9th September 2002
//	Updated:	20th August 2003	-	Init function cleaned up, caller must now specify all
//										attributes and flags
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef PBUFFER_H
#define PBUFFER_H

class PBUFFER
{
public:
	HGLRC hRC;						//rendering context
	HDC hDC;						//device context

	HPBUFFERARB hBuffer;			//buffer handle

	int width, height;				//pBuffer size
		
	bool Init(	int newWidth, int newHeight,
				const int * attribIList, const float * attribFList, const int * flags);

	void Shutdown(void);

	bool MakeCurrent(void);
};

#endif	//PBUFFER_H