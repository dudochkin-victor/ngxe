//////////////////////////////////////////////////////////////////////
// Dan Jenkins 
// 215116-2
//
//	Common structs, constants, and typedefs
//////////////////////////////////////////////////////////////////////

#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>

struct Point3
{
	float x, y, z;
};

struct Face
{
	unsigned int v1, v2, v3;
};

typedef std::vector<Point3> Point3List;
typedef std::vector<Face> FaceList;

const double PI = 3.141592654;

const int HALF_UPPER = 0x01;
const int HALF_LOWER = 0x02;
const int HALF_LEFT = 0x04;
const int HALF_RIGHT = 0x08;
const int QUAD_UPPER_LEFT = HALF_UPPER | HALF_LEFT;
const int QUAD_UPPER_RIGHT = HALF_UPPER | HALF_RIGHT;
const int QUAD_LOWER_LEFT = HALF_LOWER | HALF_LEFT;
const int QUAD_LOWER_RIGHT = HALF_LOWER | HALF_RIGHT;
const int CENTER_UPPER = HALF_UPPER | HALF_LEFT | HALF_RIGHT;
const int CENTER_LOWER = HALF_LOWER | HALF_LEFT | HALF_RIGHT;
const int CENTER_LEFT = HALF_LEFT | HALF_UPPER | HALF_LOWER;
const int CENTER_RIGHT = HALF_RIGHT | HALF_UPPER | HALF_LOWER;
const int CENTER_ORIGIN = HALF_RIGHT | HALF_LEFT | HALF_UPPER | HALF_LOWER;

#endif

