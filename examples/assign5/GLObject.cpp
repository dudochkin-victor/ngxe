// GLObject.cpp: implementation of the CGLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "GLObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLObject::CGLObject() : 
	m_bPrecached(false), m_nDisplayList(0), 
	m_bTextured(false),	m_nTextureID(0)
{

}

CGLObject::~CGLObject()
{

}

void CGLObject::PreCache()
{
	m_bPrecached = false;
	m_nDisplayList = glGenLists(1);

	glNewList(m_nDisplayList, GL_COMPILE);
	Render();
	glEndList();

	m_bPrecached = true;

}

bool CGLObject::Render()
{
	if (m_bPrecached)
	{
		glCallList(m_nDisplayList);
		return true;
	}
	else
		return false;
}

bool CGLObject::LoadTexture(const char *filename)
{
	AUX_RGBImageRec* pImg=auxDIBImageLoad(filename);

	if (pImg)
	{
		glGenTextures(1, &m_nTextureID);

		glBindTexture(GL_TEXTURE_2D, m_nTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, pImg->sizeX,
			pImg->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImg->data);

		m_bTextured = true;
	}
	else
		return false;

	return true;
}
