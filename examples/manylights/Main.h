//////////////////////////////////////////////////////////////////////////////////////////
//	Main.h
//	Many Lights
//	Downloaded from: www.paulsprojects.net
//	Created:	15th January 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef MAIN_H
#define MAIN_H

//Set up GL
bool GLInit();

//Set up variables
bool DemoInit();

//Perform per-frame updates
void UpdateFrame();

//Render a frame
void RenderFrame(double currentTime, double timePassed);

//Shut down demo
void DemoShutdown();

//WinMain
int WINAPI WinMain(	HINSTANCE	hInstance,			//Instance
					HINSTANCE	hPrevInstance,		//Previous Instance
					LPSTR		lpCmdLine,			//Command line params
					int			nShowCmd);			//Window show state

//Simple vertex class
class SIMPLE_VERTEX
{
public:
	VECTOR3D position;
	VECTOR3D normal;
	VECTOR2D texCoords;
};

//Class for a light
class POINT_LIGHT
{
public:
	VECTOR3D position;
	VECTOR3D velocity;
	COLOR color;
};

#endif	//MAIN_H