//////////////////////////////////////////////////////////////////////////////////////////
//	WALKING_CAMERA.h
//	class declaration for walking camera
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef WALKING_CAMERA_H
#define WALKING_CAMERA_H

struct POINT {
		int x;
		int y;
	};

class WALKING_CAMERA
{
public:
	VECTOR3D position;
	float angleYaw, anglePitch;
	float speed;

	void setMousePos(int x, int y);
	void Update(int key);
	void Init(	float newSpeed,
				VECTOR3D newPosition=VECTOR3D(0.0f, 0.0f, 0.0f),
				float newAngleYaw=0.0f, float newAnglePitch=0.0f);
private:
	POINT mPos;
};

#endif	//WALKING_CAMERA_H