/*
 *  cAnimationManager.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "cAnimationManager.h"

cAnimationManager::cAnimationManager()
{
}

cAnimationManager::~cAnimationManager()
{
    RemoveAll();
}

uint cAnimationManager::CreateAnimation( std::vector< uint > Frames, std::vector< Uint32 > Delays)
{
    //check the vector sizes
    if(Frames.size() != Delays.size())
    {
        return 0;
    }

    cAnimation newAnimation;

    //set all frames and delays
    for(int i = 0; i < Frames.size(); i++)
    {
        newAnimation.SetTexture( Frames[i], i);
        newAnimation.SetDelay( Delays[i], i);
    }

    //find the place
    uint place = m_Animations.size() + 1;

    //add to the map
    m_Animations[place] = newAnimation;

	return place;
}

uint cAnimationManager::CreateAnimation( std::vector< uint > Frames, Uint32 Delay)
{
    cAnimation newAnimation;

    //set all frames and delays
    for(int i = 0; i < Frames.size(); i++)
    {
        newAnimation.SetTexture( Frames[i], i);
        newAnimation.SetDelay( Delay, i);
    }

    //find the place
    uint place = m_Animations.size() + 1;

    //add to the map
    m_Animations[place] = newAnimation;

	return place;
}

void cAnimationManager::Remove(uint Animation)
{
    m_Animations.erase(Animation);
}

void cAnimationManager::RemoveAll()
{
    m_Animations.clear();
}

void cAnimationManager::Pause(uint Animation)
{
    m_Animations[Animation].Pause();
}

void cAnimationManager::PauseAll()
{
    //loop through all
    std::map<uint, cAnimation>::iterator itr = m_Animations.begin();

    for (itr; itr != m_Animations.end(); itr++)
    {
        itr->second.Pause();
    }
}

void cAnimationManager::Update(uint Animation)
{
    m_Animations[Animation].Update();
}

void cAnimationManager::UpdateAll()
{
    //loop through all
    std::map<uint, cAnimation>::iterator itr = m_Animations.begin();

    for (itr; itr != m_Animations.end(); itr++)
    {
        itr->second.Update();
    }
}

void cAnimationManager::Resume(uint Animation)
{
    m_Animations[Animation].Resume();
}

void cAnimationManager::ResumeAll()
{
    //loop through all
    std::map<uint, cAnimation>::iterator itr = m_Animations.begin();

    for (itr; itr != m_Animations.end(); itr++)
    {
        itr->second.Resume();
    }
}

void cAnimationManager::SetRepeations(uint Animation, int reps)
{
    m_Animations[Animation].SetRepeations(reps);
}

void cAnimationManager::Reset(uint Animation)
{
    m_Animations[Animation].Reset();
}

void cAnimationManager::ResetAll()
{
    //loop through all
    std::map<uint, cAnimation>::iterator itr = m_Animations.begin();

    for (itr; itr != m_Animations.end(); itr++)
    {
        itr->second.Reset();
    }
}

void cAnimationManager::JumpToFrame(uint Animation, uint Frame)
{
    m_Animations[Animation].JumpToFrame(Frame);
}

void cAnimationManager::DrawAnimation(uint Animation, GLfloat x, GLfloat y, GLfloat Scale, GLfloat rotation, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    m_Animations[Animation].Draw(x, y, Scale, rotation, red, green, blue, alpha);
}
