//////////////////////////////////////////////////////////////////////////////////////////
//	Main.h
//	Refraction
//	Downloaded from: www.paulsprojects.net
//	Created:	1st September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef MAIN_H
#define MAIN_H

bool DemoInit();
bool GLInit();

void InitChromaticSingleStates();
void InitSimpleSingleStates();
void InitChromaticTwin1States();
void InitChromaticTwin2States();

void UpdateFrame();
void RenderFrame();
void DrawBackground();

void DemoShutdown();

int WINAPI WinMain(HINSTANCE	hInstance,
				   HINSTANCE	hPervInstance,
				   LPSTR		lpCmdLine,
				   int			nCmdShow);					//main entry point

#endif	//MAIN_H