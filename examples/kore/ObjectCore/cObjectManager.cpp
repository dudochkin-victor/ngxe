/*
 *  cObjectManager.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/19/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "cObjectManager.h"


cAnimationObjectManager::cAnimationObjectManager()
{
}

cAnimationObjectManager::~cAnimationObjectManager()
{
}

uint cAnimationObjectManager::AddAnimationObject(cAnimationObject* Object)
{
	return 0;
}

void cAnimationObjectManager::MoveObject( uint Object, GLfloat x, GLfloat y )
{
}

void cAnimationObjectManager::MoveAllObjects( GLfloat x, GLfloat y )
{
}

void cAnimationObjectManager::SetPosition( uint Object, GLfloat x, GLfloat y)
{
}

void cAnimationObjectManager::SetAnimation( uint Object, sTextureSection* Texture)
{
}

void cAnimationObjectManager::JumpToAnimationFrame(uint Object, uint Frame)
{
}

void cAnimationObjectManager::ScaleAnimationObject(uint)
{
}

void cAnimationObjectManager::UpdateAnimation(uint Object)
{
}

void cAnimationObjectManager::UpdateAllAnimations()
{
}

void cAnimationObjectManager::PauseAnimation(uint Object)
{
}

void cAnimationObjectManager::PauseAllAnimations()
{
}

void cAnimationObjectManager::ResumeAnimation(uint Object)
{
}

void cAnimationObjectManager::ResumeAllAnimations()
{
}

void cAnimationObjectManager::ResetAnimation(uint Object)
{
}

void cAnimationObjectManager::ResetAllAnimation()
{
}

void cAnimationObjectManager::DrawAnimation(uint Object)
{
}

void cAnimationObjectManager::DrawAllAnimations()
{
}

cObjectManager::cObjectManager()
{
}

cObjectManager::~cObjectManager()
{
}

uint cObjectManager::AddObject(cObject* Object)
{
	return 0;
}

void cObjectManager::MoveObject( uint Object, GLfloat x, GLfloat y )
{
}

void cObjectManager::MoveAllObjects( GLfloat x, GLfloat y )
{
}

void cObjectManager::SetPosition( uint Object, GLfloat x, GLfloat y)
{
}

void cObjectManager::ScaleObject(uint)
{
}

void cObjectManager::DrawObject(uint Object)
{
}

void cObjectManager::DrawAllObjects()
{
}
