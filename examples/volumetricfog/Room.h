// Room.h: interface for the CRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROOM_H__3E02C118_E326_43D2_A4B6_BD8880A5FDD4__INCLUDED_)
#define AFX_ROOM_H__3E02C118_E326_43D2_A4B6_BD8880A5FDD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int WALL_FLOOR = 0x01;
const int WALL_CEILING = 0x02;
const int WALL_BACK = 0x04;
const int WALL_LEFT = 0x08;
const int WALL_RIGHT = 0x10;
const int WALL_ALL = WALL_FLOOR | WALL_CEILING | WALL_BACK | WALL_LEFT | WALL_RIGHT;

const int WALL_FLOOR_INDEX = 0;
const int WALL_CEILING_INDEX = 1;
const int WALL_BACK_INDEX = 2;
const int WALL_LEFT_INDEX = 3;
const int WALL_RIGHT_INDEX = 4;

class CRoom  
{
public:
	void SetColor(int walls, float r, float g, float b, float a);
	void SetTexture(int walls, GLuint texid);
	void SetDimensions(float width, float height, float depth, float slices, float stacks);
	void Draw();
	CRoom();
	virtual ~CRoom();

private:
	void DrawWall(int nWallIndex);

private:
	GLuint m_pTextures[5];
	GLfloat* m_pVertices;
	GLfloat* m_pTexCoords;
	GLuint** m_pVertexIndices[5];
	GLuint m_nStacks;
	GLuint m_nSlices;
	GLfloat m_pColors[5][4];
};

#endif // !defined(AFX_ROOM_H__3E02C118_E326_43D2_A4B6_BD8880A5FDD4__INCLUDED_)
