// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EBDE5F0C_C932_4528_9768_30286E27E5C7__INCLUDED_)
#define AFX_STDAFX_H__EBDE5F0C_C932_4528_9768_30286E27E5C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// TODO: reference additional headers your program requires here
#include <GL/glut.h>
#ifdef WIN32
#include <GL/glaux.h>
#else
struct AUX_RGBImageRec
{
	int sizeX;
	int sizeY;
	GLubyte* data;
};

AUX_RGBImageRec* auxDIBImageLoad(const char* filename);
#endif
#include <cmath>
#include <fstream>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EBDE5F0C_C932_4528_9768_30286E27E5C7__INCLUDED_)
