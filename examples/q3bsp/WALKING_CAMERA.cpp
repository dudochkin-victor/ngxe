//////////////////////////////////////////////////////////////////////////////////////////
//	WALKING_CAMERA.cpp
//	functions for walking camera
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
//#include <windows.h>
#include <GL/gl.h>
#include "WINDOW.h"
#include "LOG.h"
#include "Maths/Maths.h"
#include "WALKING_CAMERA.h"

extern LOG errorLog;
extern WINDOW window;

void WALKING_CAMERA::Init(float newSpeed, VECTOR3D newPosition, float newAngleYaw, float newAnglePitch)
{
	speed=newSpeed;
	position=newPosition;
	angleYaw=newAngleYaw;
	anglePitch=newAnglePitch;
//	SetCursorPos(320, 240);
}

struct POINT {
	int x;
	int y;
};

void WALKING_CAMERA::Update(double time)
{
	//calculate the distance to move, based on time passed
	static double lastTime=time;
	double timePassed=time-lastTime;
	lastTime=time;

	float distance=speed*(float)timePassed/1000;

	//Get the mouse position
	POINT mPos;
//	GetCursorPos(&mPos);
	
	angleYaw+=((float)mPos.x-320.0f)*speed/20;
	anglePitch+=((float)mPos.y-240.0f)*speed/20;

	//make sure angleY is not too great
	if(anglePitch>80.0f)
		anglePitch=80.0f;
	
	if(anglePitch<-80.0f)
		anglePitch=-80.0f;

	//set the mouse back to the centre of the screen
//	SetCursorPos(320,240);

//	//move forward/back or strafe
//	if(window.isKeyPressed(VK_UP) || window.isKeyPressed('W'))
//	{
//		position.x += (float)sin(angleYaw*M_PI/180)*distance;
//		position.z -= (float)cos(angleYaw*M_PI/180)*distance;
//	}
//
//	if(window.isKeyPressed(VK_DOWN) || window.isKeyPressed('S'))
//	{
//		position.x -= (float)sin(angleYaw*M_PI/180)*distance;
//		position.z += (float)cos(angleYaw*M_PI/180)*distance;
//	}
//
//	if(window.isKeyPressed(VK_RIGHT) || window.isKeyPressed('D'))
//	{
//		position.x += (float)cos(angleYaw*M_PI/180)*distance;
//		position.z += (float)sin(angleYaw*M_PI/180)*distance;
//	}
//
//	if(window.isKeyPressed(VK_LEFT) || window.isKeyPressed('A'))
//	{
//		position.x -= (float)cos(angleYaw*M_PI/180)*distance;
//		position.z -= (float)sin(angleYaw*M_PI/180)*distance;
//	}
}
