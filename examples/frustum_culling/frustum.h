#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED

#include "plane.h"

class Geometry;

class FrustumCulling {
	public:
		
	void prepare();
	bool isVisible(const Geometry &object) const;

	private:

	typedef enum { LEFT,RIGHT,BOTTOM,TOP,ZNEAR,ZFAR } Face;

  Plane m_frustum[6];

	bool isVisible(const Vector3f &point) const;
};

#endif // FRUSTUM_H_INCLUDED