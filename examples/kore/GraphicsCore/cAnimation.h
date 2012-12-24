/*
 *  cAnimation.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef ANIMATION
#define ANIMATION

#include "sTexture.h"
#include "../globals.h"

class cAnimation
{
	private:
			std::vector< uint > m_Textures;
			std::vector< Uint32 > m_Delays;

			uint m_CurrentFrame;

			Uint32 m_LastTime;

			bool m_Paused;
			int m_Repeations;

	public:
			cAnimation();
			~cAnimation();

			void Update();
			void Pause();
			void Resume();
			void Reset();
			void JumpToFrame(uint Frame);

			void SetRepeations(int Repeations);
			void SetTexture(uint Texture, uint position);
			void SetDelay(Uint32 delay, uint position);
			void Draw(GLfloat x, GLfloat y, GLfloat Scale = 1, GLfloat rotation= 0, GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1, GLfloat alpha = 1);

			uint GetFrameCount();
};

#endif
