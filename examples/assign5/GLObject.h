// GLObject.h: interface for the CGLObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBJECT_H__D5D9F7B3_CF83_4E06_8F91_283F1C8AFC1F__INCLUDED_)
#define AFX_GLOBJECT_H__D5D9F7B3_CF83_4E06_8F91_283F1C8AFC1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common.h"

class CGLObject  
{
public:
	bool LoadTexture(const char* filename);
	virtual bool Render();
	void PreCache();
	CGLObject();
	virtual ~CGLObject();
	void SetPosition(Point3 pos) { m_vecPos = pos; }
protected:
	Point3 m_vecPos;
	bool m_bPrecached;
	unsigned int m_nDisplayList;
	bool m_bTextured;
	unsigned int m_nTextureID;
};

#endif // !defined(AFX_GLOBJECT_H__D5D9F7B3_CF83_4E06_8F91_283F1C8AFC1F__INCLUDED_)
