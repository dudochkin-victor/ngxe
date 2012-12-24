//////////////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
//
// RotationalObj.cpp: implementation of the CRotationalObj class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "RotationalObj.h"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRotationalObj::CRotationalObj()
:m_pLists(NULL)
{
	m_vecPos.x = 0.0;
	m_vecPos.y = 0.0;
	m_vecPos.z = 0.0;
}

CRotationalObj::~CRotationalObj()
{
	if (m_pLists)
		delete [] m_pLists;
}

// Calculates the vertices of the rotated set of points
void CRotationalObj::RotateLine(Point3List points, int nSegments)
{
	if (m_pLists)
		delete [] m_pLists;

	if (nSegments > 0)
	{
		m_pLists = new Point3List[nSegments + 1];

		m_pLists[0] = points;

		float fSegAngle = (2*PI)/static_cast<float>(nSegments);

		for (int c=1; c<=nSegments; c++)
		{
			m_pLists[c].resize(m_pLists[c-1].size());

			for (int n=0; n<m_pLists[c].size(); n++)
			{
				float fPrevAngle = GetAngle(m_pLists[c-1][n].x, m_pLists[c-1][n].z);
				float fLength = sqrt(m_pLists[c-1][n].x*m_pLists[c-1][n].x + m_pLists[c-1][n].z*m_pLists[c-1][n].z);
				Point3 newPoint;

				fPrevAngle += fSegAngle;
				
				GetPoint(fPrevAngle, fLength, newPoint.x, newPoint.z);

				newPoint.y = m_pLists[c-1][n].y;

				m_pLists[c][n] = newPoint;
				
			}
		}
	}

	m_nSegments = nSegments;
}

// Render the object
bool CRotationalObj::Render()
{
	glPushMatrix();
	glTranslatef(m_vecPos.x, m_vecPos.y, m_vecPos.z);


	if (!CGLObject::Render())
	{
		int c;
		// Find the middle of the object, x and y will always be 0

		glBindTexture(GL_TEXTURE_2D, m_nTextureID);

		for (c=0;c<m_nSegments;c++)
		{
			// Use a series of triangle strips to minimize function 
			// call overhead.
			glBegin(GL_TRIANGLE_STRIP);
			for (int n=0;n<m_pLists[c].size();n++)
			{
				Point3 norm1, norm2;
				float len;

				norm1 = m_pLists[c+1][n];
				norm2 = m_pLists[c][n];

				norm1.y = 0.0;
				norm2.y = 0.0;

				if (n > 0)
				{
					norm1.x += m_pLists[c+1][n].x - m_pLists[c+1][n-1].x;
					norm1.y += m_pLists[c+1][n].y - m_pLists[c+1][n-1].y;
					norm1.z += m_pLists[c+1][n].z - m_pLists[c+1][n-1].z;

					norm2.x += m_pLists[c][n].x - m_pLists[c][n-1].x;
					norm2.y += m_pLists[c][n].y - m_pLists[c][n-1].y;
					norm2.z += m_pLists[c][n].z - m_pLists[c][n-1].z;
				}

				if (n < m_pLists[c].size() - 1)
				{
					norm1.x += m_pLists[c+1][n].x - m_pLists[c+1][n+1].x;
					norm1.y += m_pLists[c+1][n].y - m_pLists[c+1][n+1].y;
					norm1.z += m_pLists[c+1][n].z - m_pLists[c+1][n+1].z;

					norm2.x += m_pLists[c][n].x - m_pLists[c][n+1].x;
					norm2.y += m_pLists[c][n].y - m_pLists[c][n+1].y;
					norm2.z += m_pLists[c][n].z - m_pLists[c][n+1].z;
				}

				len = sqrt(norm1.x*norm1.x + norm1.y*norm1.y + norm1.z*norm1.z);
				norm1.x/=len;
				norm1.y/=len;
				norm1.z/=len;

				len = sqrt(norm2.x*norm2.x + norm2.y*norm2.y + norm2.z*norm2.z);
				norm2.x/=len;
				norm2.y/=len;
				norm2.z/=len;

				if (m_bTextured)
				{
					float s, t;

					s = static_cast<float>(c+1)/static_cast<float>(m_nSegments);
					t = static_cast<float>(n)/static_cast<float>(m_pLists[c+1].size());
					glTexCoord2f(s,t);
				}

				glNormal3f(norm1.x, norm1.y, norm1.z);
				glVertex3f(m_pLists[c+1][n].x, m_pLists[c+1][n].y, m_pLists[c+1][n].z);

				if (m_bTextured)
				{
					float s, t;

					s = static_cast<float>(c)/static_cast<float>(m_nSegments);
					t = static_cast<float>(n)/static_cast<float>(m_pLists[c].size());
					glTexCoord2f(s,t);
				}

				glNormal3f(norm2.x, norm2.y, norm2.z);
				glVertex3f(m_pLists[c][n].x, m_pLists[c][n].y, m_pLists[c][n].z);

			}
			glEnd();
		}
	}
	glPopMatrix();

	return true;
}

// Calculates the angle of a point in the y plane given an x/z position
float CRotationalObj::GetAngle(float x, float z)
{
	// Setup a bit flag for the quadrant of the angle
	int quad = (x <= 0 ? HALF_LEFT : 0) | (x >= 0 ? HALF_RIGHT : 0) |
		(z <= 0 ? HALF_LOWER : 0) | (z >= 0 ? HALF_UPPER :0);
	float fAngle = 0.0;

	switch(quad)
	{
	case QUAD_UPPER_LEFT:
		fAngle = atan(z/-x) + PI + PI/2;
		break;
	case QUAD_UPPER_RIGHT:
		fAngle = atan(x/z);
		break;
	case QUAD_LOWER_LEFT:
		fAngle = atan(-x/-z) + PI;
		break;
	case QUAD_LOWER_RIGHT:
		fAngle = atan(-z/x) + PI/2;
		break;
	case CENTER_ORIGIN:
	case CENTER_UPPER:
		fAngle = 0.0;
		break;
	case CENTER_LOWER:
		fAngle = PI;
		break;
	case CENTER_RIGHT:
		fAngle = PI/2;
		break;
	case CENTER_LEFT:
		fAngle = PI + PI/2;
		break;
	}

	return fAngle;
}

// Calculates the x/z positions given an angle and length for the y plane
void CRotationalObj::GetPoint(float angle, float length, float &x, float &z)
{
	// Make sure the angle is between 0 and 2PI
	if (angle < 0)
		angle += 2*PI;

	if (angle > 2*PI)
		angle -= 2*PI * static_cast<int>(angle/(2*PI));

	// Setup a bit field to calculate the quadrant for the point
	int quad = (angle >=0 && angle <= PI ? QUAD_UPPER_RIGHT : 0) | 
		(angle >= PI && angle <= 180 ? QUAD_LOWER_RIGHT : 0) |
		(angle >= 180 && angle <= 270 ? QUAD_LOWER_LEFT : 0) |
		(angle >= 270 && angle <= 360 ? QUAD_UPPER_LEFT : 0);

	x=0; z=0;

	if (length > 0)
	{
		switch(quad)
		{
		case QUAD_UPPER_LEFT:
			x = -length * cos(angle-(3*PI/2));
			z = length * sin(angle-(3*PI/2));
			break;
		case QUAD_UPPER_RIGHT:
			x = length * sin(angle);
			z = length * cos(angle);
			break;
		case QUAD_LOWER_LEFT:
			x = -length * sin(angle-PI);
			z = -length * cos(angle-PI);
			break;
		case QUAD_LOWER_RIGHT:
			x = length * cos(angle-PI/2);
			z = -length * sin(angle-PI/2);
			break;
		case CENTER_ORIGIN:
			x=0;
			z=0;
			break;
		case CENTER_UPPER:
			x=0;
			z=length;
			break;
		case CENTER_LOWER:
			x=0;
			z=-length;
			break;
		case CENTER_RIGHT:
			x=length;
			z=0;
			break;
		case CENTER_LEFT:
			x=-length;
			z=0;
			break;
		}
	}
}
