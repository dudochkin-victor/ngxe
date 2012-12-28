// Room.cpp: implementation of the CRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Room.h"

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoom::CRoom()
: m_pVertices(NULL), m_pTexCoords(NULL), m_nStacks(0), m_nSlices(0)
{
	for (int c=0;c<5;c++)
	{
		m_pVertexIndices[c] = NULL;
		m_pTextures[c] = 0;
		m_pColors[c][0] = 1.0;
		m_pColors[c][1] = 1.0;
		m_pColors[c][2] = 1.0;
		m_pColors[c][3] = 1.0;
	}
}

CRoom::~CRoom()
{
	for (int c=0;c<5;c++)
	{
		if (m_pVertexIndices[c])
		{
			for (GLuint d=0;d<m_nStacks;d++)
				delete [] m_pVertexIndices[c][d];

			delete [] m_pVertexIndices[c];
		}
	}

	if (m_pVertices)
		delete [] m_pVertices;

	if (m_pTexCoords)
		delete [] m_pTexCoords;
}

void CRoom::Draw()
{
	GLuint oldTex = 0;
	for (int c=0;c<5;c++)
	{
		glColor4fv(m_pColors[c]);
		if (oldTex != m_pTextures[c])
		{
			glBindTexture(GL_TEXTURE_2D, m_pTextures[c]);
			oldTex = m_pTextures[c];
		}
		DrawWall(c);
	}

}

void CRoom::SetDimensions(float width, float height, float depth, float slices, float stacks)
{
	int nVerts = (slices + 1) * (stacks + 1);
	float left, right, top, bottom, back, front;
	int x, y, c, nVertIndex ;

	left = -width/2.0;
	right = -left;
	top = height;
	bottom = 0;
	back = -depth/2.0;
	front = -back;

	m_pVertices = new GLfloat[nVerts * 3 * 5];
	m_pTexCoords = new GLfloat[nVerts * 2 * 5];

	for (c=0;c<5;c++)
		m_pVertexIndices[c] = new GLuint*[stacks];

	for (y = 0;y<=stacks;y++)
	{
		if (y<stacks)
		{
			for (c=0;c<5;c++)
				m_pVertexIndices[c][y] = new GLuint[(slices + 1) * 2];
		}

		for (x=0, nVertIndex=0;x<=slices;x++,nVertIndex+=2)
		{
			int offset = (y*(slices + 1) + x);
			float xOff = static_cast<float>(x)/static_cast<float>(slices) * width;
			float yOff = static_cast<float>(y)/static_cast<float>(stacks) * height;
			float zOff = static_cast<float>(y)/static_cast<float>(stacks) * depth;

			// Bottom
			m_pVertices[3*(nVerts * WALL_FLOOR_INDEX + offset)] = left + xOff;
			m_pVertices[3*(nVerts * WALL_FLOOR_INDEX + offset) + 1] = bottom;
			m_pVertices[3*(nVerts * WALL_FLOOR_INDEX + offset) + 2] = back + zOff;

			m_pTexCoords[2*(nVerts * WALL_FLOOR_INDEX + offset)] = xOff / width;
			m_pTexCoords[2*(nVerts * WALL_FLOOR_INDEX + offset) + 1] = zOff / depth;

			if (y<stacks)
			{
				m_pVertexIndices[WALL_FLOOR_INDEX][y][nVertIndex] = nVerts * WALL_FLOOR_INDEX + offset;
				m_pVertexIndices[WALL_FLOOR_INDEX][y][nVertIndex+1] = nVerts * WALL_FLOOR_INDEX + offset + slices + 1;
			}

			// Top
			m_pVertices[3*(nVerts * WALL_CEILING_INDEX + offset)] = left + xOff;
			m_pVertices[3*(nVerts * WALL_CEILING_INDEX + offset) + 1] = top;
			m_pVertices[3*(nVerts * WALL_CEILING_INDEX + offset) + 2] = front - zOff;
	
			m_pTexCoords[2*(nVerts * WALL_CEILING_INDEX + offset)] = xOff / width;
			m_pTexCoords[2*(nVerts * WALL_CEILING_INDEX + offset) + 1] = zOff / depth;

			if (y<stacks)
			{
				m_pVertexIndices[WALL_CEILING_INDEX][y][nVertIndex] = nVerts * WALL_CEILING_INDEX + offset;
				m_pVertexIndices[WALL_CEILING_INDEX][y][nVertIndex+1] = nVerts * WALL_CEILING_INDEX + offset + slices + 1;
			}

			// Back
			m_pVertices[3*(nVerts * WALL_BACK_INDEX + offset)] = left + xOff;
			m_pVertices[3*(nVerts * WALL_BACK_INDEX + offset) + 1] = top - yOff;
			m_pVertices[3*(nVerts * WALL_BACK_INDEX + offset) + 2] = back;
			
			m_pTexCoords[2*(nVerts * WALL_BACK_INDEX + offset)] = xOff / width;
			m_pTexCoords[2*(nVerts * WALL_BACK_INDEX + offset) + 1] = yOff / height;

			if (y<stacks)
			{
				m_pVertexIndices[WALL_BACK_INDEX][y][nVertIndex] = nVerts * WALL_BACK_INDEX + offset;
				m_pVertexIndices[WALL_BACK_INDEX][y][nVertIndex+1] = nVerts * WALL_BACK_INDEX + offset + slices + 1;
			}

			// Left
			m_pVertices[3*(nVerts * WALL_LEFT_INDEX + offset)] = left;
			m_pVertices[3*(nVerts * WALL_LEFT_INDEX + offset) + 1] = top - yOff;
			m_pVertices[3*(nVerts * WALL_LEFT_INDEX + offset) + 2] = front - xOff;

			m_pTexCoords[2*(nVerts * WALL_LEFT_INDEX + offset)] = xOff / width;
			m_pTexCoords[2*(nVerts * WALL_LEFT_INDEX + offset) + 1] = yOff / height;

			if (y<stacks)
			{
				m_pVertexIndices[WALL_LEFT_INDEX][y][nVertIndex] = nVerts * WALL_LEFT_INDEX + offset;
				m_pVertexIndices[WALL_LEFT_INDEX][y][nVertIndex+1] = nVerts * WALL_LEFT_INDEX + offset + slices + 1;
			}

			// Right
			m_pVertices[3*(nVerts * WALL_RIGHT_INDEX + offset)] = right;
			m_pVertices[3*(nVerts * WALL_RIGHT_INDEX + offset) + 1] = top - yOff;
			m_pVertices[3*(nVerts * WALL_RIGHT_INDEX + offset) + 2] = back + xOff;

			m_pTexCoords[2*(nVerts * WALL_RIGHT_INDEX + offset)] = xOff / width;
			m_pTexCoords[2*(nVerts * WALL_RIGHT_INDEX + offset) + 1] = yOff / height;

			if (y<stacks)
			{
				m_pVertexIndices[WALL_RIGHT_INDEX][y][nVertIndex] = nVerts * WALL_RIGHT_INDEX + offset;
				m_pVertexIndices[WALL_RIGHT_INDEX][y][nVertIndex+1] = nVerts * WALL_RIGHT_INDEX + offset + slices + 1;
			}
		}
	}

	m_nStacks = stacks;
	m_nSlices = slices;

	glVertexPointer(3, GL_FLOAT, 0, m_pVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, m_pTexCoords);

}

void CRoom::SetTexture(int walls, GLuint texid)
{
	if (walls & WALL_FLOOR)
		m_pTextures[WALL_FLOOR_INDEX] = texid;
	if (walls & WALL_CEILING)
		m_pTextures[WALL_CEILING_INDEX] = texid;
	if (walls & WALL_BACK)
		m_pTextures[WALL_BACK_INDEX] = texid;
	if (walls & WALL_LEFT)
		m_pTextures[WALL_LEFT_INDEX] = texid;
	if (walls & WALL_RIGHT)
		m_pTextures[WALL_RIGHT_INDEX] = texid;
}

void CRoom::SetColor(int walls, float r, float g, float b, float a)
{
	if (walls & WALL_FLOOR)
	{
		m_pColors[WALL_FLOOR_INDEX][0] = r;
		m_pColors[WALL_FLOOR_INDEX][1] = g;
		m_pColors[WALL_FLOOR_INDEX][2] = b;
		m_pColors[WALL_FLOOR_INDEX][3] = a;
	}

	if (walls & WALL_CEILING)
	{
		m_pColors[WALL_CEILING_INDEX][0] = r;
		m_pColors[WALL_CEILING_INDEX][1] = g;
		m_pColors[WALL_CEILING_INDEX][2] = b;
		m_pColors[WALL_CEILING_INDEX][3] = a;
	}

	if (walls & WALL_BACK)
	{
		m_pColors[WALL_BACK_INDEX][0] = r;
		m_pColors[WALL_BACK_INDEX][1] = g;
		m_pColors[WALL_BACK_INDEX][2] = b;
		m_pColors[WALL_BACK_INDEX][3] = a;
	}

	if (walls & WALL_LEFT)
	{
		m_pColors[WALL_LEFT_INDEX][0] = r;
		m_pColors[WALL_LEFT_INDEX][1] = g;
		m_pColors[WALL_LEFT_INDEX][2] = b;
		m_pColors[WALL_LEFT_INDEX][3] = a;
	}

	if (walls & WALL_RIGHT)
	{
		m_pColors[WALL_RIGHT_INDEX][0] = r;
		m_pColors[WALL_RIGHT_INDEX][1] = g;
		m_pColors[WALL_RIGHT_INDEX][2] = b;
		m_pColors[WALL_RIGHT_INDEX][3] = a;
	}

}

void CRoom::DrawWall(int nWallIndex)
{
	for (GLuint c=0;c<m_nStacks;c++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, (m_nSlices+1)*2, GL_UNSIGNED_INT,
			m_pVertexIndices[nWallIndex][c]);
	}
}

