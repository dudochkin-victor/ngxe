//////////////////////////////////////////////////////////////////////////////////////////
//	SHADOW_MODEL.cpp
//	functions for shadow-volume model
//	Downloaded from: www.paulsprojects.net
//	Created:	3rd September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include "LOG.h"
#include "Maths/Maths.h"
#include "SHADOW_MODEL.h"

extern LOG errorLog;

bool SHADOW_MODEL::GenerateTorus(float innerRadius, float outerRadius)
{
	int torusPrecision=24;

	numVertices=(torusPrecision+1)*(torusPrecision+1);
	numIndices=2*torusPrecision*torusPrecision*3;
	numTriangles=numIndices/3;

	vertices=new SHADOW_MODEL_VERTEX[numVertices];
	if(!vertices)
	{
		errorLog.OutputError("Unable to allocate memory for torus vertices");
		return false;
	}

	indices=new unsigned int[numIndices];
	if(!indices)
	{
		errorLog.OutputError("Unable to allocate memory for torus indices");
		return false;
	}
	
	//calculate the first ring - inner radius 4, outer radius 1.5
	for(int i=0; i<torusPrecision+1; i++)
	{
		vertices[i].position=VECTOR3D(innerRadius, 0.0f, 0.0f).GetRotatedZ(i*360.0f/torusPrecision)+
									VECTOR3D(outerRadius, 0.0f, 0.0f);
		
		vertices[i].normal=VECTOR3D(0.0f, 0.0f, 1.0f).
							CrossProduct(VECTOR3D(0.0f, -1.0f, 0.0f).GetRotatedZ(i*360.0f/torusPrecision));
	}

	//rotate this to get other rings
	for(int ring=1; ring<torusPrecision+1; ring++)
	{
		for(int i=0; i<torusPrecision+1; i++)
		{
			vertices[ring*(torusPrecision+1)+i].position=vertices[i].position.GetRotatedY(ring*360.0f/torusPrecision);
			
			vertices[ring*(torusPrecision+1)+i].normal=vertices[i].normal.GetRotatedY(ring*360.0f/torusPrecision);
		}
	}

	//calculate the indices
	for(int ring=0; ring<torusPrecision; ring++)
	{
		for(int i=0; i<torusPrecision; i++)
		{
			indices[((ring*torusPrecision+i)*2)*3+0]=ring*(torusPrecision+1)+i;
			indices[((ring*torusPrecision+i)*2)*3+1]=(ring+1)*(torusPrecision+1)+i;
			indices[((ring*torusPrecision+i)*2)*3+2]=ring*(torusPrecision+1)+i+1;
			indices[((ring*torusPrecision+i)*2+1)*3+0]=ring*(torusPrecision+1)+i+1;
			indices[((ring*torusPrecision+i)*2+1)*3+1]=(ring+1)*(torusPrecision+1)+i;
			indices[((ring*torusPrecision+i)*2+1)*3+2]=(ring+1)*(torusPrecision+1)+i+1;
		}
	}


	//Calculate the plane equation for each face
	planeEquations=new PLANE[numTriangles];
	if(!planeEquations)
	{
		errorLog.OutputError("Unable to allocate memory for %d planes", numTriangles);
		return false;
	}

	for(unsigned int j=0; j<numTriangles; ++j)
	{
		planeEquations[j].SetFromPoints(vertices[indices[j*3+0]].position,
										vertices[indices[j*3+1]].position,
										vertices[indices[j*3+2]].position);
	}

	//Create space for the "is facing light" booleans
	isFacingLight=new bool[numTriangles];
	if(!isFacingLight)
	{
		errorLog.OutputError("Unable to allocate memory for %d booleans", numTriangles);
		return false;
	}

	//Create space for connectivity data
	neighbourIndices=new GLint[numTriangles*3];
	if(!neighbourIndices)
	{
		errorLog.OutputError("Unable to allocate memory for %d neighbour indices", numTriangles*3);
		return false;
	}

	//Create space for "is silhouette edge" booleans
	isSilhouetteEdge=new bool[numTriangles*3];
	if(!isSilhouetteEdge)
	{
		errorLog.OutputError("Unable to allocate memory for %d booleans", numTriangles*3);
		return false;
	}

	//Calculate the neighbours
	SetConnectivity();

	return true;
}

//Calculate neighbour faces for each edge
void SHADOW_MODEL::SetConnectivity()
{
	//set the neighbour indices to be -1
	for(unsigned int i=0; i<numTriangles*3; ++i)
		neighbourIndices[i]=-1;

	//loop through triangles
	for(unsigned int i=0; i<numTriangles-1; ++i)
	{
		//loop through edges on the first triangle
		for(int edgeI=0; edgeI<3; ++edgeI)
		{
			//continue if this edge already has a neighbour set
			if(neighbourIndices[i*3+edgeI]!=-1)
				continue;

			//loop through triangles with greater indices than this one
			for(unsigned int j=i+1; j<numTriangles; ++j)
			{
				//loop through edges on triangle j
				for(int edgeJ=0; edgeJ<3; ++edgeJ)
				{
					//get the vertex indices on each edge
					int edgeI1=indices[i*3+edgeI];
					int edgeI2=indices[i*3+(edgeI+1)%3];
					int edgeJ1=indices[j*3+edgeJ];
					int edgeJ2=indices[j*3+(edgeJ+1)%3];

					//if these are the same (possibly reversed order), these faces are neighbours
					if(		(edgeI1==edgeJ1 && edgeI2==edgeJ2)
						||	(edgeI1==edgeJ2 && edgeI2==edgeJ1))
					{
						neighbourIndices[i*3+edgeI]=j;
						neighbourIndices[j*3+edgeJ]=i;
					}
				}
			}
		}
	}
}

//calculate silhouette edges
void SHADOW_MODEL::CalculateSilhouetteEdges(VECTOR3D lightPosition)
{
	//Calculate which faces face the light
	for(unsigned int i=0; i<numTriangles; ++i)
	{
		if(planeEquations[i].ClassifyPoint(lightPosition)==POINT_IN_FRONT_OF_PLANE)
			isFacingLight[i]=true;
		else
			isFacingLight[i]=false;
	}

	//loop through edges
	for(unsigned int i=0; i<numTriangles*3; ++i)
	{
		//if this face is not facing the light, not a silhouette edge
		if(!isFacingLight[i/3])
		{
			isSilhouetteEdge[i]=0;
			continue;
		}

		//this face is facing the light
		//if the neighbouring face is not facing the light, or there is no neighbouring face,
		//then this is a silhouette edge
		if(neighbourIndices[i]==-1 || !isFacingLight[neighbourIndices[i]])
		{
			isSilhouetteEdge[i]=1;
			continue;
		}

		isSilhouetteEdge[i]=0;
	}
}

void SHADOW_MODEL::DrawInfiniteShadowVolume(VECTOR3D lightPosition, bool drawCaps)
{
	glBegin(GL_QUADS);
	{
		for(unsigned int i=0; i<numTriangles; ++i)
		{
			//if this face does not face the light, continue
			if(!isFacingLight[i])
				continue;

			//Loop through edges on this face
			for(int j=0; j<3; ++j)
			{
				//Draw the shadow volume "edge" if this is a silhouette edge
				if(isSilhouetteEdge[i*3+j])
				{
					VECTOR3D vertex1=vertices[indices[i*3+j]].position;
					VECTOR3D vertex2=vertices[indices[i*3+(j+1)%3]].position;

					glVertex3fv(vertex2);
					glVertex3fv(vertex1);
					glVertex4f(	vertex1.x-lightPosition.x,
								vertex1.y-lightPosition.y,
								vertex1.z-lightPosition.z, 0.0f);
					glVertex4f(	vertex2.x-lightPosition.x,
								vertex2.y-lightPosition.y,
								vertex2.z-lightPosition.z, 0.0f);
				}
			}
		}
	}
	glEnd();

	//Draw caps if required
	if(drawCaps)
	{
		glBegin(GL_TRIANGLES);
		{
			for(unsigned int i=0; i<numTriangles; ++i)
			{
				for(int j=0; j<3; ++j)
				{
					VECTOR3D vertex=vertices[indices[i*3+j]].position;
					
					if(isFacingLight[i])
						glVertex3fv(vertex);
					else
						glVertex4f(	vertex.x-lightPosition.x,
									vertex.y-lightPosition.y,
									vertex.z-lightPosition.z, 0.0f);
				}
			}
		}
		glEnd();
	}
}
