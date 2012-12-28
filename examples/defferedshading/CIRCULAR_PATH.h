//////////////////////////////////////////////////////////////////////////////////////////
//	CIRCULAR_PATH.h
//	Class declaration for a circular path to be followed by an object
//	Downloaded from: www.paulsprojects.net
//	Created:	19th August 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef CIRCULAR_PATH_H
#define CIRCULAR_PATH_H

class CIRCULAR_PATH
{
public:
	//Initiate the path, given the centre, start position and another point in the plane
	//of movement
	bool Init(	const VECTOR3D & newCentre,
				const VECTOR3D & newStart,
				const VECTOR3D & pointInPlane);

	//Get the position on the path given the angle round the circle
	VECTOR3D GetPosition(float angle);

protected:
	//Centre of circle
	VECTOR3D centre;

	//2 orthonormal vectors in the plane of movement
	//u points in the direction from the centre to the start point
	VECTOR3D u, v;

	//Radius of circle
	float radius;
};

#endif