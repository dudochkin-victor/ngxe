//////////////////////////////////////////////////////////////////////////////////////////
//	InitLightTextures.h
//	Function to prepare attenuation textures for per-pixel attenuation
//	Downloaded from: www.paulsprojects.net
//	Created:	19th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef INITLIGHTTEXTURES_H
#define INITLIGHTTEXTURES_H

bool InitLightTextures(GLuint & atten1DTexture, GLuint & atten2DTexture, GLuint & atten3DTexture,
					   GLuint & gaussian1DTexture, GLuint & gaussian2DTexture);

#endif	//INITLIGHTTEXTURES_H