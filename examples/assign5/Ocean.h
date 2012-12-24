//////////////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
//
// Ocean.h: interface for the COcean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCEAN_H__21A338F6_008C_4CA9_BF1C_1DC6106E0B2C__INCLUDED_)
#define AFX_OCEAN_H__21A338F6_008C_4CA9_BF1C_1DC6106E0B2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common.h"
#include "GLObject.h"

class COcean  : public CGLObject
{
public:
	bool Render();
	void Create(float width, float depth, int segments, float scale);
	COcean();
	virtual ~COcean();
private:
	Point3List m_listVerts;
	Point3List m_listNormals;
};

#endif // !defined(AFX_OCEAN_H__21A338F6_008C_4CA9_BF1C_1DC6106E0B2C__INCLUDED_)
