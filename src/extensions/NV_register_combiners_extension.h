//////////////////////////////////////////////////////////////////////////////////////////
//	NV_register_combiners_extension.h
//	Extension setup header
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef NV_REGISTER_COMBINERS_EXTENSION_H
#define NV_REGISTER_COMBINERS_EXTENSION_H

bool SetUpNV_register_combiners();
extern bool NV_register_combiners_supported;

extern PFNGLCOMBINERPARAMETERFVNVPROC				glCombinerParameterfvNV;
extern PFNGLCOMBINERPARAMETERIVNVPROC				glCombinerParameterivNV;
extern PFNGLCOMBINERPARAMETERFNVPROC				glCombinerParameterfNV;
extern PFNGLCOMBINERPARAMETERINVPROC				glCombinerParameteriNV;

extern PFNGLCOMBINERINPUTNVPROC						glCombinerInputNV;
extern PFNGLCOMBINEROUTPUTNVPROC					glCombinerOutputNV;

extern PFNGLFINALCOMBINERINPUTNVPROC				glFinalCombinerInputNV;

extern PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC		glGetCombinerInputParameterfvNV;
extern PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC		glGetCombinerInputParameterivNV;

extern PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC		glGetCombinerOutputParameterfvNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC		glGetCombinerOutputParameterivNV;

extern PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC	glGetFinalCombinerInputParameterfvNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC	glGetFinalCombinerInputParameterivNV;

#endif	//NV_REGISTER_COMBINERS_EXTENSION_H
