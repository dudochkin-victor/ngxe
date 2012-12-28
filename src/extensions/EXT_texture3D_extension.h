//////////////////////////////////////////////////////////////////////////////////////////
//	EXT_texture3D_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef EXT_TEXTURE3D_EXTENSION_H
#define EXT_TEXTURE3D_EXTENSION_H

bool SetUpEXT_texture3D();
extern bool EXT_texture3D_supported;

extern PFNGLTEXIMAGE3DEXTPROC						glTexImage3DEXT;

#endif	//EXT_TEXTURE3D_EXTENSION_H
