//////////////////////////////////////////////////////////////////////////////////////////
//	POINT_LIGHT.h
//	Class declaration for a point light following a circular path
//	Downloaded from: www.paulsprojects.net
//	Created:	19th August 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class POINT_LIGHT
{
public:
	//Initiate the light
	bool Init(	const VECTOR3D & newPathCentre,
				const VECTOR3D & newPathStart,
				const VECTOR3D & pointInPathPlane,
				float newSpeed,
				const COLOR & newColor,
				float newRadius);

	//Get current position
	VECTOR3D GetPosition(double currentTime);

	//Get the rectangle in window coordinates which just encloses the region of influence
	void GetWindowRect(	int windowWidth, int windowHeight,
						const MATRIX4X4 & cameraViewMatrix, double currentTime,
						float cameraNearDistance, float cameraFovy, float cameraAspectRatio,
						int & x, int & y, int & width, int & height);

protected:
	CIRCULAR_PATH path;
	float speed;

public:
	COLOR color;
	float radius;
};

#endif