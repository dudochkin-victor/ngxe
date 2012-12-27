//////////////////////////////////////////////////////////////////////////////////////////
//	FRUSTUM.h
//	class declaration for frustum for frustum culling
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef FRUSTUM_H
#define FRUSTUM_H

//planes of frustum
enum FRUSTUM_PLANES
{
	LEFT_PLANE=0,
	RIGHT_PLANE,
	TOP_PLANE,
	BOTTOM_PLANE,
	NEAR_PLANE,
	FAR_PLANE
};

enum FRUSTUM_CLASSIFICATION
{
	OUTSIDE_FRUSTUM=0,
	IN_FRUSTUM
};

class FRUSTUM
{
public:
	void Update();
	bool IsPointInside(const VECTOR3D & point);
	bool IsBoundingBoxInside(const VECTOR3D * vertices);
	
protected:
	PLANE planes[6];
};

#endif	//FRUSTUM_H