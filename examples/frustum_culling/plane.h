#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "vector.h"

#ifdef _DEBUG
  #include <iostream.h>
#endif

class Plane {
	private:

	Vector3f m_normal;
	float m_distance;

	public:

	Plane(const Vector3f &normal=Vector3f::Y(),float distance=0):m_normal(normal),m_distance(distance) { normalize(); }

	float getA() const { return m_normal.getX(); }
	float getB() const { return m_normal.getY(); }
	float getC() const { return m_normal.getZ(); }
	float getD() const { return m_distance; }

	float getDistanceTo(const Vector3f &point) const { return (m_normal%point)+m_distance; }

  // Ensures the normal vector length (magnitude) equals one.
	void normalize() { float length=m_normal.getLength(); m_normal/=length; m_distance/=length;	}

  #ifdef _DEBUG
  /// Reads a plane from an input stream.
  friend istream& operator>>(istream& s,Plane& p) { return s >> p.m_normal >> p.m_distance; }
  /// Writes a plane to an output stream.
  friend ostream& operator<<(ostream& s,const Plane& p) { return s << '(' << p.m_normal << ',' << p.m_distance << ')'; }
  #endif
};

#endif // PLANE_H_INCLUDED