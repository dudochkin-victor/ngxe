/*
 *  cObject.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef OBJECT
#define OBJECT

#include "../Graphics Core/cAnimationManager.h"
#include "../Graphics Core/GraphicsCore.h"

class cObject
{
	public:
			cObject();
			cObject(GLfloat x, GLfloat y, uint TextureSection);
			~cObject();

			void Move( GLfloat x, GLfloat y );
			void SetPosition( GLfloat x, GLfloat y);
			void SetTextureSection(uint Texture);
			void Scale(GLfloat x, GLfloat y);

			void SetVisable(bool Visable);

			void Draw();
	private:
				GLfloat m_Xscale;
				GLfloat m_Yscale;
				GLfloat m_Xpos;
				GLfloat m_Ypos;
				uint m_TextureSection;
				bool m_Visable;
};

class cAnimationObject
{
	public:
			cAnimationObject();
			cAnimationObject(GLfloat x, GLfloat y, uint Animation);
			~cAnimationObject();

			void Move( GLfloat x, GLfloat y );
			void SetPosition( GLfloat x, GLfloat y);
			void SetAnimation(uint Animation);
			void Scale(GLfloat x, GLfloat y);

			void Draw();

			void UpdateAnimation();
			void PauseAnimation();
			void ResumeAnimation();
			void ResetAnimation();
			void JumpToAnimationFrame(uint Frame);
			void SetVisable(bool Visable);
	private:
				GLfloat m_Xscale;
				GLfloat m_Yscale;
				GLfloat m_Xpos;
				GLfloat m_Ypos;
				uint m_Animation;
				bool m_Visable;
};

#endif
