//////////////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
//
// Ocean.cpp: implementation of the COcean class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Ocean.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COcean::COcean()
{
	m_vecPos.x = 0.0;
	m_vecPos.y = 0.0;
	m_vecPos.z = 0.0;
}

COcean::~COcean()
{

}

// Creates a surface with a sin curve
void COcean::Create(float width, float depth, int segments, float scale)
{
	float start = -depth/2;
	float step = depth/segments;

	for (int c=0;c<segments;c++)
	{
		Point3 p1, p2;
		Point3 norm;

		p1.x = width/2;
		p2.x = -p1.x;

		p1.z = p2.z = start + c*step;
		p1.y = p2.y = sin(p1.z*scale)/scale;
		
		// Set the normal (negative reciprocal of the derivative)
		norm.x = 0.0;
		norm.y = 1;
		norm.z = -cos(p1.z*scale);

		m_listVerts.push_back(p1);
		m_listVerts.push_back(p2);
		m_listNormals.push_back(norm);
		m_listNormals.push_back(norm);
	}
}

// Renders the sin curve surface
bool COcean::Render()
{
	glPushMatrix();
	glTranslatef(m_vecPos.x, m_vecPos.y, m_vecPos.z);

	if (!CGLObject::Render())
	{
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);

		// Use a triangle strip to reduce function calls
		glBegin(GL_TRIANGLE_STRIP);
		for (int c=0;c<m_listVerts.size(); c++)
		{
			if (m_bTextured)
			{
				float s, t;

				s = c % 2;
				t = (c - s) / m_listVerts.size();

				glTexCoord2f(s, t);
			}

			glNormal3f(m_listNormals[c].x, m_listNormals[c].y, m_listNormals[c].z);
			glVertex3f(m_listVerts[c].x, m_listVerts[c].y, m_listVerts[c].z);
		}
		glEnd();
	}
	glPopMatrix();

	return true;
}

