//////////////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
//
// RotationalObj.h: interface for the CRotationalObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROTATIONALOBJ_H__7206C6D1_07A1_4CAE_B2E1_CD989D333AF9__INCLUDED_)
#define AFX_ROTATIONALOBJ_H__7206C6D1_07A1_4CAE_B2E1_CD989D333AF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common.h"
#include "GLObject.h"

class CRotationalObj  : public CGLObject
{
public:
	bool Render();
	void RotateLine(Point3List points, int nSegments);
	CRotationalObj();
	virtual ~CRotationalObj();
private:
	float GetAngle(float x, float z);
	void GetPoint(float angle, float length, float& x, float& z);

	Point3List* m_pLists;
	unsigned int m_nSegments;

};

#endif // !defined(AFX_ROTATIONALOBJ_H__7206C6D1_07A1_4CAE_B2E1_CD989D333AF9__INCLUDED_)
