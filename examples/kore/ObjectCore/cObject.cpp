/*
 *  cObject.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "cObject.h"
#include "../Graphics Core/GraphicsCore.h"

//cObject

cObject::cObject()
{
    m_Visable = 1;
    m_Xscale = 1;
    m_Yscale = 1;
    m_Xpos = 0;
    m_Ypos = 0;
    m_TextureSection = 0;
}

cObject::cObject(GLfloat x, GLfloat y, uint TextureSection)
{
    m_Visable = 1;
    m_Xscale = 1;
    m_Yscale = 1;
    m_Xpos = x;
    m_Ypos = y;
    m_TextureSection = TextureSection;
}

cObject::~cObject()
{
}

void cObject::Move( GLfloat x, GLfloat y )
{
    m_Xpos += x;
    m_Ypos += y;
}

void cObject::SetPosition( GLfloat x, GLfloat y)
{
    m_Xpos = x;
    m_Ypos = y;
}

void cObject::SetTextureSection(uint TextureSection)
{
    m_TextureSection = TextureSection;
}

void cObject::Scale(GLfloat x, GLfloat y)
{
    m_Xscale = x;
    m_Yscale = y;
}

void cObject::SetVisable(bool Visable)
{
    m_Visable = Visable;
}

void cObject::Draw()
{
    g_TextureManager->DrawTextureSection(m_TextureSection, m_Xpos , m_Ypos);
}

// cAnimationObject

cAnimationObject::cAnimationObject()
{
    m_Xscale = 1;
    m_Yscale = 1;
	m_Xpos = 0;
	m_Ypos = 0;
	m_Animation = 0;
	m_Visable = 1;
}

cAnimationObject::cAnimationObject(GLfloat x, GLfloat y, uint Animation)
{
    m_Xscale = 1;
    m_Yscale = 1;
	m_Xpos = x;
	m_Ypos = y;
	m_Animation = Animation;
	m_Visable = 1;
}

cAnimationObject::~cAnimationObject()
{
}

void cAnimationObject::Move( GLfloat x, GLfloat y )
{
    m_Xpos += x;
	m_Ypos += y;
}

void cAnimationObject::SetPosition( GLfloat x, GLfloat y)
{
    m_Xpos = x;
	m_Ypos = y;
}

void cAnimationObject::SetAnimation(uint Animation)
{
    m_Animation = Animation;
}

void cAnimationObject::Scale(GLfloat x, GLfloat y)
{
    m_Xscale = 1;
    m_Yscale = 1;
}

void cAnimationObject::Draw()
{
    g_AnimationManager->DrawAnimation(m_Animation, m_Xpos, m_Ypos);
}

void cAnimationObject::UpdateAnimation()
{
}

void cAnimationObject::PauseAnimation()
{
}

void cAnimationObject::ResumeAnimation()
{
}

void cAnimationObject::ResetAnimation()
{
}

void cAnimationObject::JumpToAnimationFrame(uint Frame)
{
}
