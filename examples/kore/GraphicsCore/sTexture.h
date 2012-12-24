/*
 *  sTexture.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef TEXTURE
#define TEXTURE

#include "../globals.h"

struct sSection
{
    //u for width and v for height
	GLfloat uMin;
	GLfloat uMax;
	GLfloat vMin;
	GLfloat vMax;
};

struct sTexture
{
	GLuint Texture;
	std::string Filename;
	GLfloat Width;
	GLfloat Height;
};

struct sTextureSection
{
	uint TextureId;
	sSection Section;
	GLfloat Width;
	GLfloat Height;
};

#endif
