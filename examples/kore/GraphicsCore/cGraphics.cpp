/*
 *  cGraphics.cpp
 *  Kore
 *
 *  Created by Sean Chapel on 11/21/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "cGraphics.h"
#include "GraphicsCore.h"

cGraphics::cGraphics()
{
    m_Loaded = false;
}

cGraphics::~cGraphics()
{
	Shutdown();
}

bool cGraphics::Initialize(int Width, int Height)
{
    if(!m_Loaded)
    {
        g_AnimationManager = new cAnimationManager;
        g_TextureManager = new cTextureManager;
        g_FontManager = new cFontManager;
        g_Camera = new cCamera;
    }

    //setup ortho mode
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, (GLfloat)Width, 0, (GLfloat)Height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//if we relizialize then reload the textures
	if(m_Loaded)
	{
        g_TextureManager->ReloadTextures();
	}


    m_Loaded = true;

	return 1;
}

void cGraphics::Shutdown()
{
	delete g_Camera;
	delete g_FontManager;
	delete g_TextureManager;
	delete g_AnimationManager;
}
