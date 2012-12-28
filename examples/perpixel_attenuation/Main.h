//////////////////////////////////////////////////////////////////////////////////////////
//	Main.h
//	Per Pixel Attenuation
//	Downloaded from: www.paulsprojects.net
//	Created:	25th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef MAIN_H
#define MAIN_H


//quad vertices
static GLfloat texCoords[]={	1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
								1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
								0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
								1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0};

static GLfloat vertexPositions[]={	-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
									 1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,
									-1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
									 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
									-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
									 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
									-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
									 1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
									 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,
									 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
									-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
									-1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f};


bool DemoInit();
bool GLInit();

void SetTextureAttenStates();
void SetPassThroughAttenStates();
void SetTexture3DAttenStates();
void SetGaussianAttenStates();

void UpdateFrame();
void RenderFrame();

void DemoShutdown();

int WINAPI WinMain(HINSTANCE	hInstance,
				   HINSTANCE	hPervInstance,
				   LPSTR		lpCmdLine,
				   int			nCmdShow);					//main entry point

#endif	//MAIN_H