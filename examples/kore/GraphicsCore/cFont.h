/*
 *  cFont.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef FONTS
#define FONTS

#include "sTexture.h"

struct sGlyph
{
    char Glyph;
    uint TextureSection;
};

struct sFont
{
    GLfloat Spacing;
    GLfloat GlyphWidth;
    GLfloat GlyphHeight;
    std::string Name;
    uint Texture;
    std::map<char, sGlyph> Gylphs;
};

#endif
