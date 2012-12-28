//////////////////////////////////////////////////////////////////////////////////////////
//	TORUS_KNOT.h
//	Class declaration for torus knot
//	Downloaded from: www.paulsprojects.net
//	Created:	14th August 2003
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef TORUS_KNOT_H
#define TORUS_KNOT_H

class TORUS_KNOT_VERTEX
{
public:
	VECTOR3D position;
	VECTOR3D normal;
};

class TORUS_KNOT
{
public:
	bool Init(int turns, int slices, int stacks);

	int numVertices;
	int numIndices;

	GLuint * indices;
	TORUS_KNOT_VERTEX * vertices;

	TORUS_KNOT()
	{}
	~TORUS_KNOT()
	{
		if(indices)
			delete [] indices;
		indices=NULL;

		if(vertices)
			delete [] vertices;
		vertices=NULL;
	}
};

#endif	//TORUS_KNOT_H
