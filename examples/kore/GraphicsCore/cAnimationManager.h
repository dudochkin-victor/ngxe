/*
 *  cAnimationManager.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef ANIMMAN
#define ANIMMAN

#include "cAnimation.h"

class cAnimationManager
{
	public:
			cAnimationManager();
			~cAnimationManager();

			uint CreateAnimation( std::vector< uint > Frames, std::vector< Uint32 > Delays);
			uint CreateAnimation( std::vector< uint > Frames, Uint32 Delay);

			void Remove(uint Animation);
			void RemoveAll();

			void Pause(uint Animation);
			void PauseAll();

			void Update(uint Animation);
			void UpdateAll();

			void Resume(uint Animation);
			void ResumeAll();

			void SetRepeations(uint Animation, int reps);

			void Reset(uint Animation);
			void ResetAll();

			void JumpToFrame(uint Animation, uint Frame);

			void DrawAnimation(uint Animation, GLfloat x, GLfloat y, GLfloat Scale = 1, GLfloat rotation = 0, GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1, GLfloat alpha = 1);

	private:
			std::map< uint, cAnimation > m_Animations;
};

#endif
