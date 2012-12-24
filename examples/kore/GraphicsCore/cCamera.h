/*
 *  cCamera.h
 *  Kore
 *
 *  Created by Sean Chapel on 11/21/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef CAMERA
#define CAMERA

#include "../globals.h"

class cCamera
{
	public:
			cCamera();
			~cCamera();

			void Move(GLfloat x, GLfloat y);
			void SetPosition(GLfloat x, GLfloat y);

			GLfloat GetXposition();
			GLfloat GetYposition();
	private:
				GLfloat m_Xpos;
				GLfloat m_Ypos;
};

#endif
