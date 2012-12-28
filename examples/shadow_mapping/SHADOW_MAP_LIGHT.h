//////////////////////////////////////////////////////////////////////////////////////////
//	SHADOW_MAP_LIGHT.h
//	Class declaration for light with shadow map. Derives from INTERACTOR, so it can be moved
//	using the mouse
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SHADOW_MAP_LIGHT_H
#define SHADOW_MAP_LIGHT_H

class SHADOW_MAP_LIGHT : public INTERACTOR
{
public:
	float n, f;
	MATRIX4X4 projectionMatrix;

	void SetClipDistances(float newNear, float newFar)
	{
		n=newNear;
		f=newFar;

		UpdateMatrices();
	}

	virtual void Update();
	void UpdateMatrices(void);
	void ShrinkFrustum(void);
	void GrowFrustum(void);
};

#endif	//SHADOW_MAP_LIGHT_H