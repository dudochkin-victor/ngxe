//////////////////////////////////////////////////////////////////////////////////////////
//	SHADOW_MAP_LIGHT.cpp
//	Functions for light with shadow map
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "LOG.h"
#include "WINDOW.h"
#include "Maths/Maths.h"
#include "INTERACTOR.h"
#include "SHADOW_MAP_LIGHT.h"

extern WINDOW window;

void SHADOW_MAP_LIGHT::Update()
{
	//update angles
	if(window.isLeftButtonPressed())
	{
		leftRightRotation-=(window.GetMouseXMovement())*rotationSensitivity;
		upDownRotation-=(window.GetMouseYMovement())*rotationSensitivity;
	}

	//update radius
	if(window.isRightButtonPressed())
	{
		r+=(window.GetMouseYMovement())*(float)translationSensitivity;
		if(r<4.0)
			r=4.0;
		if(r>20.0f)
			r=20.0f;
	}

	//update position if there has been a change
	if(window.isLeftButtonPressed() || window.isRightButtonPressed())
	{
		static MATRIX4X4 viewMatrix;
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(lookAt.x, lookAt.y, lookAt.z);
		glRotated(-leftRightRotation, 0.0f, 1.0f, 0.0f);
		glRotated(-upDownRotation, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, r);
		glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
		position=viewMatrix.GetColumn(3);
		glPopMatrix();
	}

	UpdateMatrices();
}

void SHADOW_MAP_LIGHT::UpdateMatrices(void)
{
	glPushMatrix();
	SetupViewMatrix();
	
	glLoadIdentity();
	gluPerspective(60.0f, 1.0f, n, f);
	glGetFloatv(GL_MODELVIEW_MATRIX, projectionMatrix);
	glPopMatrix();
}

void SHADOW_MAP_LIGHT::ShrinkFrustum(void)
{
	if(n<2.0f)
	{
		n+=0.005f;
		f-=0.2f;
	}	
}

void SHADOW_MAP_LIGHT::GrowFrustum(void)
{
	if(n>0.055f)
	{
		n-=0.005f;
		f+=0.2f;
	}
}
