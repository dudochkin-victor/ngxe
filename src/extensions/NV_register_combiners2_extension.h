//////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners2_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef NV_REGISTER_COMBINERS2_EXTENSION_H
#define NV_REGISTER_COMBINERS2_EXTENSION_H

bool SetUpNV_register_combiners2();
extern bool NV_register_combiners2_supported;

extern PFNGLCOMBINERSTAGEPARAMETERFVNVPROC			glCombinerStageParameterfvNV;
extern PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC		glGetCombinerStageParameterfvNV;

#endif	//NV_REGISTER_COMBINERS2_EXTENSION_H
