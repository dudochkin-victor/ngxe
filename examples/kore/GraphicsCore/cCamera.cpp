/*
 *  cCamera.cpp
 *  Kore
 *
 *  Created by Sean Chapel on 11/21/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "cCamera.h"

cCamera::cCamera()
{
	m_Xpos = 0;
	m_Ypos = 0;
}

cCamera::~cCamera()
{
}

void cCamera::Move(GLfloat x, GLfloat y)
{
	m_Xpos += x;
	m_Ypos += y;
}

void cCamera::SetPosition(GLfloat x, GLfloat y)
{
	m_Xpos = x;
	m_Ypos = y;
}

GLfloat cCamera::GetXposition()
{
	return m_Xpos;
}

GLfloat cCamera::GetYposition()
{
	return m_Ypos;
}
