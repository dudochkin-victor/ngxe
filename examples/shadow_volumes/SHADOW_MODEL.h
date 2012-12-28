//////////////////////////////////////////////////////////////////////////////////////////
//	SHADOW_MODEL.h
//	class declaration for shadow volume equipped model
//	Downloaded from: www.paulsprojects.net
//	Created:	3rd September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef SHADOW_MODEL_H
#define SHADOW_MODEL_H

class SHADOW_MODEL_VERTEX
{
public:
	VECTOR3D position;
	VECTOR3D normal;
};

class SHADOW_MODEL
{
public:
	GLuint numIndices;
	GLuint * indices;

	GLuint numTriangles;

	GLuint numVertices;
	SHADOW_MODEL_VERTEX * vertices;

	//Store the plane equation for each face
	PLANE * planeEquations;

	//For each face, does it face the light?
	bool * isFacingLight;

	//For each edge, which is the neighbouring face?
	GLint * neighbourIndices;

	//For each edge, is it a silhouette edge?
	bool * isSilhouetteEdge;

	bool GenerateTorus(float innerRadius, float outerRadius);

	void SetConnectivity();
	void CalculateSilhouetteEdges(VECTOR3D lightPosition);
	void DrawInfiniteShadowVolume(VECTOR3D lightPosition, bool drawCaps);
};

#endif	//SHADOW_MODEL_H