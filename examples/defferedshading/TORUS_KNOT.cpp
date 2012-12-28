//////////////////////////////////////////////////////////////////////////////////////////
//	TORUS_KNOT.cpp
//	Functions for torus knot
//	Downloaded from: www.paulsprojects.net
//	Created:	14th August 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <windows.h>
#include <GL\gl.h>
#include "Maths/Maths.h"
#include "Log/LOG.h"
#include "TORUS_KNOT.h"

bool TORUS_KNOT::Init(int turns, int slices, int stacks)
{
	//Calculate the total number of vertices and reserve space
	numVertices=stacks*slices;
	vertices=new TORUS_KNOT_VERTEX[numVertices];
	if(!vertices)
	{
		LOG::Instance()->OutputError("Unable to create space for %d torus knot vertices",
										numVertices);
		return false;
	}

	//Calculate the position of the centre of each ring
	VECTOR3D * ringCentres=new VECTOR3D[stacks];
	if(!ringCentres)
	{
		LOG::Instance()->OutputError("Unable to create space for %d VECTOR3Ds", stacks+1);
		return false;
	}

	for(int i=0; i<stacks; ++i)
	{
		//Calculate the value of the parameter t at this point
		double t=i*2*M_PI/stacks;

		//Calculate the position
		ringCentres[i].Set(	float((1.0+0.3*cos(turns*t))*cos(2*t)),
							float((1.0+0.3*cos(turns*t))*0.3f*sin(turns*t)),
							float((1.0+0.3*cos(turns*t))*sin(2*t)));
	}

	//Loop through the rings
	for(int i=0; i<stacks; ++i)
	{
		//Loop through the vertices making up this ring
		for(int j=0; j<slices; ++j)
		{
			//Calculate the number of this vertex
			int vertexNumber=i*slices+j;

			//Get the vector from the centre of this ring to the centre of the next
			VECTOR3D tangent;

			if(i==stacks-1)
				tangent=ringCentres[0]-ringCentres[i];
			else
				tangent=ringCentres[i+1]-ringCentres[i];

			//Calculate the vector perpendicular to the tangent, pointing approximately
			//in the positive Y direction
			VECTOR3D temp1=VECTOR3D(0.0f, 1.0f, 0.0f).CrossProduct(tangent);
			VECTOR3D temp2=tangent.CrossProduct(temp1);
			temp2.Normalize();
			temp2*=0.2f;
						
			//Rotate this about the tangent vector to form the ring
			vertices[vertexNumber].position=ringCentres[i]+
												temp2.GetRotatedAxis(j*360.0f/slices, tangent);
		}
	}


	//Calculate the total number of indices and reserve space
	numIndices=6*stacks*slices;
	indices=new GLuint[numIndices];
	if(!indices)
	{
		LOG::Instance()->OutputError("Unable to create space for %d indices", numIndices);
		return false;
	}



	//Calculate the indices
	for(int i=0; i<stacks; ++i)
	{
		for(int j=0; j<slices; ++j)
		{
			//Get the index for the 4 vertices around this "quad"
			GLuint quadIndices[4];

			quadIndices[0]=i*slices+j;

			if(j!=slices-1)
				quadIndices[1]=i*slices+j+1;
			else
				quadIndices[1]=i*slices;

			if(i!=stacks-1)
				quadIndices[2]=(i+1)*slices+j;
			else
				quadIndices[2]=j;

			if(i!=stacks-1)
			{
				if(j!=slices-1)
					quadIndices[3]=(i+1)*slices+j+1;
				else
					quadIndices[3]=(i+1)*slices;
			}
			else
			{
				if(j!=slices-1)
					quadIndices[3]=j+1;
				else
					quadIndices[3]=0;
			}


			indices[(i*slices+j)*6  ]=quadIndices[0];
			indices[(i*slices+j)*6+1]=quadIndices[1];
			indices[(i*slices+j)*6+2]=quadIndices[2];

			indices[(i*slices+j)*6+3]=quadIndices[3];
			indices[(i*slices+j)*6+4]=quadIndices[2];
			indices[(i*slices+j)*6+5]=quadIndices[1];
		}
	}

	
	
	//Clear the normals
	for(int i=0; i<numVertices; ++i)
		vertices[i].normal.LoadZero();

	//Loop through the triangles
	for(int i=0; i<numIndices; i+=3)
	{
		//Calculate the normal for this triangle
		PLANE trianglePlane;
		trianglePlane.SetFromPoints(vertices[indices[i  ]].position, 
									vertices[indices[i+1]].position, 
									vertices[indices[i+2]].position);

		VECTOR3D normal=trianglePlane.GetNormal();

		//Add this to each vertex
		for(int j=0; j<3; ++j)
			vertices[indices[i+j]].normal+=normal;
	}

	//Normalize the normals
	for(int i=0; i<numVertices; ++i)
		vertices[i].normal.Normalize();

	//Delete the ringCentres array
	if(ringCentres)
		delete [] ringCentres;
	ringCentres=NULL;

	return true;
}