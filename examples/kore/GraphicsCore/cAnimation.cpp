/*
 *  cAnimation.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "cAnimation.h"
#include "GraphicsCore.h"

cAnimation::cAnimation()
{
    m_CurrentFrame = 0;
    m_LastTime = SDL_GetTicks();
    m_Paused = false;
    m_Repeations = -1;
}

cAnimation::~cAnimation()
{
}

void cAnimation::Update()
{
    //if the animation is paused skip updating
    if(m_Paused == true)
    {
        return;
    }

    //find out if enough time has passed
    if( m_Delays[m_CurrentFrame] < (SDL_GetTicks() - m_LastTime) )
    {
        m_CurrentFrame++;
        m_LastTime = SDL_GetTicks();

        //if we reached the end
        if(m_CurrentFrame >= m_Textures.size())
        {
            //if not going for infinitey then set the reps one less
            if( m_Repeations != -1 )
            {
                //if there are no reps left then pause the animation
                if( m_Repeations == 0)
                {
                    m_Paused = true;
                }
                else //handle it normaly
                {
                    m_Repeations--;
                }
            }

            m_CurrentFrame = 0;
        }
    }
}

void cAnimation::Pause()
{
    m_Paused = true;
}

void cAnimation::Resume()
{
    m_Paused = true;
    m_LastTime = SDL_GetTicks();
}

void cAnimation::Reset()
{
    m_CurrentFrame = 0;
    m_LastTime = SDL_GetTicks();
}

void cAnimation::JumpToFrame(uint Frame)
{
    //only set if the frame is in range
    if(Frame < m_Textures.size())
    {
        m_CurrentFrame = Frame;
    }

    m_LastTime = SDL_GetTicks();
}

void cAnimation::SetRepeations(int Repeations)
{
    m_Repeations = Repeations;
}

void cAnimation::SetTexture(uint Texture, uint position)
{
    //if the position is out of the range see if we can add it
    if(position >= m_Textures.size())
    {
        //if we are adding to the end
        if(position == m_Textures.size())
        {
            //add the texture then add a default delay
            m_Textures.push_back(Texture);
            m_Delays.push_back(100);
        }
    }
    else // just set it normaly
    {
        m_Textures[position] = Texture;
    }

}

void cAnimation::SetDelay(Uint32 delay, uint position)
{
    //only set a delay for a frame that exists
    if( position < m_Delays.size() )
    {
        m_Delays[position] = delay;
    }
}

void cAnimation::Draw(GLfloat x, GLfloat y, GLfloat Scale, GLfloat rotation, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    g_TextureManager->DrawTextureSection( m_Textures[m_CurrentFrame], x, y, Scale, rotation, red, green, blue, alpha);
}

uint cAnimation::GetFrameCount()
{
    return m_Textures.size();
}
