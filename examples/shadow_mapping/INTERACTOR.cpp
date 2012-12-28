//////////////////////////////////////////////////////////////////////////////////////////
//	INTERACTOR.h
//	Functions for mouse interactor class
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include "LOG.h"
#include "WINDOW.h"
#include "Maths/Maths.h"
#include "INTERACTOR.h"

extern WINDOW window;
extern LOG errorLog;

void INTERACTOR::Init(	VECTOR3D startPosition,
						VECTOR3D newLookAt)
{
	position=startPosition;
	lookAt=newLookAt;
	
	//calculate r and angles
	VECTOR3D lineOfSight=position-lookAt;
	r=lineOfSight.GetLength();
	
	if(lineOfSight.z==0.0f)
	{
		if(lineOfSight.x>0.0f)
			leftRightRotation=-90.0f;
		if(lineOfSight.x<0.0f)
			leftRightRotation=90.0f;
	}
	else
	{
		if(lineOfSight.z<0.0f)
			leftRightRotation=180.0-(180.0/M_PI)*atan(lineOfSight.x/lineOfSight.z);
		if(lineOfSight.z>0.0f)
			leftRightRotation=-(180.0/M_PI)*atan(lineOfSight.x/lineOfSight.z);
	}


	
	if(lineOfSight.x==0.0f && lineOfSight.z==0.0f)
		upDownRotation=90.0;
	else
		upDownRotation=(180.0/M_PI)*atan(lineOfSight.y/sqrt((lineOfSight.x*lineOfSight.x)+
															(lineOfSight.z*lineOfSight.z)));
}

void INTERACTOR::Update()
{
	//update angles
	if(window.isLeftButtonPressed())
	{
		leftRightRotation+=(window.GetMouseXMovement())*rotationSensitivity;
		upDownRotation+=(window.GetMouseYMovement())*rotationSensitivity;
	}

	//update radius
	if(window.isRightButtonPressed())
	{
		r+=(window.GetMouseYMovement())*(float)translationSensitivity;
	}

	//update position if there has been a change
	if(window.isLeftButtonPressed() || window.isRightButtonPressed())
	{
		static MATRIX4X4 tempMatrix;
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(lookAt.x, lookAt.y, lookAt.z);
		glRotated(-leftRightRotation, 0.0f, 1.0f, 0.0f);
		glRotated(-upDownRotation, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, r);
		glGetFloatv(GL_MODELVIEW_MATRIX, tempMatrix);
		position=tempMatrix.GetColumn(3);
		glPopMatrix();
	}
}

void INTERACTOR::SetupViewMatrix()
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -r);
	glRotated(upDownRotation, 1.0f, 0.0f, 0.0f);
	glRotated(leftRightRotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(-lookAt.x, -lookAt.y, -lookAt.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	glPopMatrix();
}
