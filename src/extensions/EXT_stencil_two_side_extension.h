//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_stencil_two_side_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	30th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef EXT_STENCIL_TWO_SIDE_EXTENSION_H
#define EXT_STENCIL_TWO_SIDE_EXTENSION_H

bool SetUpEXT_stencil_two_side();
extern bool EXT_stencil_two_side_supported;

extern PFNGLACTIVESTENCILFACEEXTPROC				glActiveStencilFaceEXT;

#endif	//EXT_STENCIL_TWO_SIDE_EXTENSION_H
