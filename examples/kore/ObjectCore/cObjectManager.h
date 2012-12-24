/*
 *  cObjectManager.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/19/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef OBJMAN
#define OBJMAN

#include "cObject.h"

class cAnimationObjectManager
{
	public:
			cAnimationObjectManager();
			~cAnimationObjectManager();

			uint AddAnimationObject(cAnimationObject* Object);

			void MoveObject( uint Object, GLfloat x, GLfloat y );
			void MoveAllObjects( GLfloat x, GLfloat y );

			void SetPosition( uint Object, GLfloat x, GLfloat y);
			void SetAnimation( uint Object, sTextureSection* Texture);
			void JumpToAnimationFrame(uint Object, uint Frame);
			void ScaleAnimationObject(uint);
			void SetVisable(uint Object, bool Visable);

			void UpdateAnimation(uint Object);
			void UpdateAllAnimations();

			void PauseAnimation(uint Object);
			void PauseAllAnimations();

			void ResumeAnimation(uint Object);
			void ResumeAllAnimations();

			void ResetAnimation(uint Object);
			void ResetAllAnimation();

			void DrawAnimation(uint Object);
			void DrawAllAnimations();

	private:
			std::vector< cAnimationObject* > m_AnimationObjects;
			std::list< uint >  m_AnimationFreeSlots;
};

class cObjectManager
{
	public:
			cObjectManager();
			~cObjectManager();

			uint AddObject(cObject* Object);

			void MoveObject( uint Object, GLfloat x, GLfloat y );
			void MoveAllObjects( GLfloat x, GLfloat y );

			void SetPosition( uint Object, GLfloat x, GLfloat y);
			void ScaleObject(uint);

			void DrawObject(uint Object);
			void DrawAllObjects();

			void SetVisable(uint Object, bool Visable);
	private:
				std::vector< cObject* > m_Objects;
				std::list< uint >  m_ObjectFreeSlots;
};

#endif
