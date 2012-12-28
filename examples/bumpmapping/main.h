//////////////////////////////////////////////////////////////////////////////////////////
//	Main.h
//	Shadow Mapping
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef MAIN_H
#define MAIN_H

bool DemoInit();
bool GLInit();

void UpdateFrame();
void RenderFrame();

void SetSinglePassStates(void);

void SetDiffuseStates(void);
void SetDecalStates(void);
void SetSimpleSpecularStates(void);

void SetDiffuseDecalStates(void);
void SetLookUpSpecularStates(void);

void DemoShutdown();
#endif	//MAIN_H
