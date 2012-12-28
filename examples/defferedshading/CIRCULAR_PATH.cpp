//////////////////////////////////////////////////////////////////////////////////////////
//	CIRCULAR_PATH.cpp
//	Functions for a circular path to be followed by an object
//	Downloaded from: www.paulsprojects.net
//	Created:	19th August 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include "Maths/Maths.h"
#include "Log/LOG.h"
#include "CIRCULAR_PATH.h"

//Initiate the path, given the centre, start position and another point in the plane of movement
bool CIRCULAR_PATH::Init(	const VECTOR3D & newCentre,
							const VECTOR3D & newStart,
							const VECTOR3D & pointInPlane)
{
	//Check that newCentre, newStart and pointInPlane are not colinear
	VECTOR3D planeNormal=(newStart-newCentre).CrossProduct(pointInPlane-newCentre);

	if(planeNormal.GetLength()<0.01f)
	{
		LOG::Instance()->OutputError("Points used to define a circular path are colinear");
		return false;
	}

	planeNormal.Normalize();

	//Save the centre, radius and u vector
	centre=newCentre;
	radius=(newStart-newCentre).GetLength();
	u=(newStart-newCentre).GetNormalized();

	//Calculate the v vector - lies within the plane, perpendicular to u
	v=u.CrossProduct(planeNormal);

	return true;
}

//Get the position on the path given the angle round the circle
VECTOR3D CIRCULAR_PATH::GetPosition(float angle)
{
	return centre + radius*u*float(cos(angle*M_PI/180)) + radius*v*float(sin(angle*M_PI/180));
}
