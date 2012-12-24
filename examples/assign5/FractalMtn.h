//////////////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
//
// FractalMtn.h: interface for the CFractalMtn class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRACTALMTN_H__F8EB3BC1_7753_4FBA_A85D_CD30DD3268DB__INCLUDED_)
#define AFX_FRACTALMTN_H__F8EB3BC1_7753_4FBA_A85D_CD30DD3268DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common.h"
#include "GLObject.h"

class CFractalMtn : public CGLObject
{
public:
	bool Render();
	void Normalize();
	void CreateFractal(Point3List listStartVerts, FaceList listStartFaces, int levels);
	CFractalMtn();
	virtual ~CFractalMtn();

private:
	void GetFaceNormal(Point3 v1, Point3 v2, Point3 v3, Point3& norm);
	void FractalSub(Point3 v1, Point3 v2, Point3 v3, int level);

	Point3List m_listVertices;
	Point3List m_listNormals;
	FaceList m_listFaces;
	Point3 m_ptMin;
	Point3 m_ptMax;
};

#endif // !defined(AFX_FRACTALMTN_H__F8EB3BC1_7753_4FBA_A85D_CD30DD3268DB__INCLUDED_)
