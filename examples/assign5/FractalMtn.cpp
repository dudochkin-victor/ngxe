//////////////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
//
// FractalMtn.cpp: implementation of the CFractalMtn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include "FractalMtn.h"

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFractalMtn::CFractalMtn()
{
	time_t t;
	time(&t);
	srand(t);

	m_vecPos.x = 0;
	m_vecPos.y = 0;
	m_vecPos.z = 0;
}

CFractalMtn::~CFractalMtn()
{
}

// Starts the recursive fractal function
void CFractalMtn::CreateFractal(Point3List listStartVerts, FaceList listStartFaces, int levels)
{
	int nFaces = listStartFaces.size();
	
	for (int c=0; c<nFaces; c++)
	{
		Point3 vec1, vec2, vec3;

		vec1 = listStartVerts[listStartFaces[c].v1];
		vec2 = listStartVerts[listStartFaces[c].v2];
		vec3 = listStartVerts[listStartFaces[c].v3];

		if (c == 0)
		{
			m_ptMin = m_ptMax = vec1;
		}

		FractalSub(vec1, vec2, vec3, levels);
	}
}

// Averages the normals of shared vertices
void CFractalMtn::Normalize()
{
	int nVerts = m_listVertices.size();
	Point3List newVerts;
	Point3List newNorms;
	std::vector<int> vertMap;

	vertMap.resize(nVerts);
	for (int c=0;c<nVerts;c++)
		vertMap[c] = -1;

	// Combine shared vertices and calculate the normal
	for (int v1=0;v1<nVerts;v1++)
	{
		if (vertMap[v1] != -1)
			continue;

		Point3 curVert = m_listVertices[v1];
		Point3 newNorm = m_listNormals[v1];
		int newIndex = newVerts.size();

		vertMap[v1] = newIndex;

		// Find all shared vertices and add normals together
		for (int v2=v1+1; v2<nVerts; v2++)
		{
			Point3 testVert = m_listVertices[v2];
			
			if (testVert.x == curVert.x && testVert.y == curVert.y && testVert.z == curVert.z)
			{
				newNorm.x += m_listNormals[v2].x;
				newNorm.y += m_listNormals[v2].y;
				newNorm.z += m_listNormals[v2].z;
				vertMap[v2] = newIndex;
			}
		}

		// Change normal size to a unit length of 1
		float len = sqrt(newNorm.x*newNorm.x + newNorm.y*newNorm.y + newNorm.z*newNorm.z);

		if (len > 0)
		{
			newNorm.x /= len;
			newNorm.y /= len;
			newNorm.z /= len;
		}

		// put new vertex and new normal in list
		newVerts.push_back(curVert);
		newNorms.push_back(newNorm);
	}

	// Set new normal and vertex lists to members
	m_listVertices = newVerts;
	m_listNormals = newNorms;

	// Remap faces to new vertices
	int nFaces = m_listFaces.size();
	
	for (int f=0; f<nFaces; f++)
	{
		m_listFaces[f].v1 = vertMap[m_listFaces[f].v1];
		m_listFaces[f].v2 = vertMap[m_listFaces[f].v2];
		m_listFaces[f].v3 = vertMap[m_listFaces[f].v3];
	}

}

// Recursive function for fractal mountain
void CFractalMtn::FractalSub(Point3 v1, Point3 v2, Point3 v3, int level)
{
	Point3 norm;

	GetFaceNormal(v1, v2, v3, norm);

	// Level is 0, add face and vertices to list
	if (level <= 0)
	{
		Face f;
		
		f.v1 = m_listVertices.size();
		f.v2 = f.v1 + 1;
		f.v3 = f.v2 + 1;

		m_listVertices.push_back(v1);
		m_listVertices.push_back(v2);
		m_listVertices.push_back(v3);

		m_listNormals.push_back(norm);
		m_listNormals.push_back(norm);
		m_listNormals.push_back(norm);

		m_listFaces.push_back(f);

		// Check if these verts go beyond the current min/max bounds
		m_ptMin.x = (m_ptMin.x < v1.x ? m_ptMin.x : v1.x);
		m_ptMin.x = (m_ptMin.x < v2.x ? m_ptMin.x : v2.x);
		m_ptMin.x = (m_ptMin.x < v3.x ? m_ptMin.x : v3.x);
		m_ptMin.y = (m_ptMin.y < v1.y ? m_ptMin.y : v1.y);
		m_ptMin.y = (m_ptMin.y < v2.y ? m_ptMin.y : v2.y);
		m_ptMin.y = (m_ptMin.y < v3.y ? m_ptMin.y : v3.y);
		m_ptMin.z = (m_ptMin.z < v1.z ? m_ptMin.z : v1.z);
		m_ptMin.z = (m_ptMin.z < v2.z ? m_ptMin.z : v2.z);
		m_ptMin.z = (m_ptMin.z < v3.z ? m_ptMin.z : v3.z);
		
		m_ptMax.x = (m_ptMax.x > v1.x ? m_ptMax.x : v1.x);
		m_ptMax.x = (m_ptMax.x > v2.x ? m_ptMax.x : v2.x);
		m_ptMax.x = (m_ptMax.x > v3.x ? m_ptMax.x : v3.x);
		m_ptMax.y = (m_ptMax.y > v1.y ? m_ptMax.y : v1.y);
		m_ptMax.y = (m_ptMax.y > v2.y ? m_ptMax.y : v2.y);
		m_ptMax.y = (m_ptMax.y > v3.y ? m_ptMax.y : v3.y);
		m_ptMax.z = (m_ptMax.z > v1.z ? m_ptMax.z : v1.z);
		m_ptMax.z = (m_ptMax.z > v2.z ? m_ptMax.z : v2.z);
		m_ptMax.z = (m_ptMax.z > v3.z ? m_ptMax.z : v3.z);
	}
	else  // Divide the face and recursively divide those faces
	{
		Point3 v12, v13, v23;
		// Random variations on the extrusion height
		float variation1 = static_cast<float>((rand() % 90)  + 10)/100.0;
		float variation2 = static_cast<float>((rand() % 90)  + 10)/100.0;
		float variation3 = static_cast<float>((rand() % 90)  + 10)/100.0;

		// Random variations for the positions where the face is divided
		float midvar1 = static_cast<float>((rand() % 33) + 33)/100.0;
		float midvar2 = static_cast<float>((rand() % 33) + 33)/100.0;
		float midvar3 = static_cast<float>((rand() % 33) + 33)/100.0;

		// Scale the x and z vector's down so the faces go up more than out
		norm.x *= 0.1;
		norm.z *= 0.1;

		// Calculate new vertex positions
		v12.x = v1.x + (v2.x - v1.x)*midvar1 + variation1*norm.x;
		v12.y = v1.y + (v2.y - v1.y)*midvar2 + variation2*norm.y;
		v12.z = v1.z + (v2.z - v1.z)*midvar3 + variation3*norm.z;

		v13.x = v1.x + (v3.x - v1.x)*midvar1 + variation1*norm.x;
		v13.y = v1.y + (v3.y - v1.y)*midvar2 + variation2*norm.y;
		v13.z = v1.z + (v3.z - v1.z)*midvar3 + variation3*norm.z;

		v23.x = v2.x + (v3.x - v2.x)*midvar1 + variation1*norm.x;
		v23.y = v2.y + (v3.y - v2.y)*midvar2 + variation2*norm.y;
		v23.z = v2.z + (v3.z - v2.z)*midvar3 + variation3*norm.z;

		// Recursively divide the faces
		FractalSub(v1, v12, v13, level-1);
		FractalSub(v12, v2, v23, level-1);
		FractalSub(v13, v23, v3, level-1);

		FractalSub(v12, v23, v13, level-1);

		FractalSub(v12, v1, v2, level-2);
		FractalSub(v23, v2, v3, level-2);
		FractalSub(v13, v3, v1, level-2);
	}
}

// Renders the mountain
bool CFractalMtn::Render()
{
	// Position the mountain
	glPushMatrix();
	glTranslatef(m_vecPos.x, m_vecPos.y, m_vecPos.z);

	if (!CGLObject::Render())
	{
		int nFaces = m_listFaces.size();

		glBindTexture(GL_TEXTURE_2D, m_nTextureID);

		// Render using triangles
		glBegin(GL_TRIANGLES);
		{
			for (int f=0; f<nFaces; f++)
			{
				int v[3];

				v[0] = m_listFaces[f].v1;
				v[1] = m_listFaces[f].v2;
				v[2] = m_listFaces[f].v3;

				for (int c=0;c<3;c++)
				{
					if (m_bTextured)
					{
						float s, t;
						float x, z;

						x = m_listVertices[v[c]].x;
						z = m_listVertices[v[c]].z;

						s = (x - m_ptMin.x) / (m_ptMax.x - m_ptMin.x);
						t = (z - m_ptMin.z) / (m_ptMax.z - m_ptMin.z);

						glTexCoord2f(s, t);
					}

					glNormal3f(m_listNormals[v[c]].x, m_listNormals[v[c]].y, m_listNormals[v[c]].z);
					glVertex3f(m_listVertices[v[c]].x, m_listVertices[v[c]].y, m_listVertices[v[c]].z);
				}
			}	
		}
		glEnd();
	}
	// Restore the matrix to the state it was in before rendering	
	glPopMatrix();

	return true;
}

// Calculates the face normal by taking the cross product of v1-v2 and v2-v3
void CFractalMtn::GetFaceNormal(Point3 v1, Point3 v2, Point3 v3, Point3 &norm)
{
	Point3 vd12, vd23;
	float length;

	// Calculate v1-v2
	vd12.x = v1.x - v2.x;
	vd12.y = v1.y - v2.y;
	vd12.z = v1.z - v2.z;

	// Calculate v2-v3
	vd23.x = v2.x - v3.x;
	vd23.y = v2.y - v3.y;
	vd23.z = v2.z - v3.z;

	// Calculate cross product
	norm.x = vd12.y*vd23.z - vd23.y*vd12.z;
	norm.y = vd23.x*vd12.z - vd12.x*vd23.z;
	norm.z = vd12.x*vd23.y - vd23.x*vd12.y;

	// Scale length of normal to 1
	length = sqrt(norm.x*norm.x + norm.y*norm.y + norm.z*norm.z);

	if (length > 0)
	{
		norm.x /= length;
		norm.y /= length;
		norm.z /= length;
	}
	else
	{
		norm.x = 0;
		norm.y = 1;
		norm.z = 0;
	}
}




