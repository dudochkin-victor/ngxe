//////////////////////////////////////////////////////////////////////////////////////////
//	POINT_LIGHT.cpp
//	Functions for a point light following a circular path
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
#include "POINT_LIGHT.h"

//Initiate the light
bool POINT_LIGHT::Init(	const VECTOR3D & newPathCentre,
						const VECTOR3D & newPathStart,
						const VECTOR3D & pointInPathPlane,
						float newSpeed,
						const COLOR & newColor,
						float newRadius)
{
	//Set up the path
	if(!path.Init(newPathCentre, newPathStart, pointInPathPlane))
		return false;

	//Save the other properties
	speed=newSpeed;
	color=newColor;
	radius=newRadius;

	return true;
}

//Get the current position
VECTOR3D POINT_LIGHT::GetPosition(double currentTime)
{
	return path.GetPosition(float(currentTime*speed));
}

//Get the rectangle in window coordinates which just encloses the region of influence
void POINT_LIGHT::GetWindowRect(int windowWidth, int windowHeight,
								const MATRIX4X4 & cameraViewMatrix, double currentTime,
								float cameraNearDistance, float cameraFovy,
								float cameraAspectRatio,
								int & x, int & y, int & width, int & height)
{
	//Begin by calculating the rectangle's coordinates in [-1, 1]x[-1, 1]
	float left=-1.0f;
	float right=1.0f;
	
	float bottom=-1.0f;
	float top=1.0f;

	//r is the radius of the  light
	float & r=radius;

	//l is the light's position in eye space
	VECTOR3D l=cameraViewMatrix*GetPosition(currentTime);

	//halfNearPlaneHeight is half the height of the near plane, i.e. from the centre to the top
	float halfNearPlaneHeight=cameraNearDistance*(float)tan(double(cameraFovy*M_PI/360));

	float halfNearPlaneWidth=halfNearPlaneHeight*cameraAspectRatio;

	//All calculations in eye space
	
	
	//We wish to find 2 planes parallel to the Y axis which are tangent to the bounding sphere
	//of the light and pass through the origin (camera position)

	//plane normal. Of the form (x, 0, z)
	VECTOR3D normal;

	//Calculate the discriminant of the quadratic we wish to solve to find nx(divided by 4)
	float d=(l.z*l.z) * ( (l.x*l.x) + (l.z*l.z) - r*r );

	//If d>0, solve the quadratic to get the normal to the plane
	if(d>0.0f)
	{
		float rootD=(float)sqrt(d);

		//Loop through the 2 solutions
		for(int i=0; i<2; ++i)
		{
			//Calculate the normal
			if(i==0)
				normal.x=r*l.x+rootD;
			else
				normal.x=r*l.x-rootD;

			normal.x/=(l.x*l.x + l.z*l.z);

			normal.z=r-normal.x*l.x;
			normal.z/=l.z;

			//We need to divide by normal.x. If ==0, no good
			if(normal.x==0.0f)
				continue;


			//p is the point of tangency
			VECTOR3D p;

			p.z=(l.x*l.x) + (l.z*l.z) - r*r;
			p.z/=l.z-((normal.z/normal.x)*l.x);

			//If the point of tangency is behind the camera, no good
			if(p.z>=0.0f)
				continue;

			p.x=-p.z * normal.z/normal.x;

			//Calculate where the plane meets the near plane
			//divide by the width to give a value in [-1, 1] for values on the screen
			float screenX=normal.z * cameraNearDistance / (normal.x*halfNearPlaneWidth);
			
			//If this is a left bounding value (p.x<l.x) and is further right than the
			//current value, update
			if(p.x<l.x && screenX>left)
				left=screenX;

			//Similarly, update the right value
			if(p.x>l.x && screenX<right)
				right=screenX;
		}
	}


	//Repeat for planes parallel to the x axis
	//normal is now of the form(0, y, z)
	normal.x=0.0f;

	//Calculate the discriminant of the quadratic we wish to solve to find ny(divided by 4)
	d=(l.z*l.z) * ( (l.y*l.y) + (l.z*l.z) - r*r );

	//If d>0, solve the quadratic to get the normal to the plane
	if(d>0.0f)
	{
		float rootD=(float)sqrt(d);

		//Loop through the 2 solutions
		for(int i=0; i<2; ++i)
		{
			//Calculate the normal
			if(i==0)
				normal.y=r*l.y+rootD;
			else
				normal.y=r*l.y-rootD;

			normal.y/=(l.y*l.y + l.z*l.z);

			normal.z=r-normal.y*l.y;
			normal.z/=l.z;

			//We need to divide by normal.y. If ==0, no good
			if(normal.y==0.0f)
				continue;


			//p is the point of tangency
			VECTOR3D p;

			p.z=(l.y*l.y) + (l.z*l.z) - r*r;
			p.z/=l.z-((normal.z/normal.y)*l.y);

			//If the point of tangency is behind the camera, no good
			if(p.z>=0.0f)
				continue;

			p.y=-p.z * normal.z/normal.y;

			//Calculate where the plane meets the near plane
			//divide by the height to give a value in [-1, 1] for values on the screen
			float screenY=normal.z * cameraNearDistance / (normal.y*halfNearPlaneHeight);
			
			//If this is a bottom bounding value (p.y<l.y) and is further up than the
			//current value, update
			if(p.y<l.y && screenY>bottom)
				bottom=screenY;

			//Similarly, update the top value
			if(p.y>l.y && screenY<top)
				top=screenY;
		}
	}

	//Now scale and bias into [0, windowWidth-1]x[0, windowHeight-1]
	int x1=int((left+1)*windowWidth/2);
	int x2=int((right+1)*windowWidth/2);

	int y1=int((bottom+1)*windowHeight/2);
	int y2=int((top+1)*windowHeight/2);

	//Clamp these values to the edge of the screen
	if(x1<0)
		x1=0;
	
	if(x2>windowWidth-1)
		x2=windowWidth;

	if(y1<0)
		y1=0;

	if(y2>windowHeight-1)
		y2=windowHeight;

	//Fill the outputs
	x=x1;
	width=x2-x1;

	y=y1;
	height=y2-y1;
}
