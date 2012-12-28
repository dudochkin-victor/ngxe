#include "frustum.h"
#include "geometry.h"
#include "matrix.h"

#include <GL/glut.h>

bool FrustumCulling::isVisible(const Vector3f &point) const {
	for (int i=5;i>=0;--i)
		if (m_frustum[i].getDistanceTo(point)<0) return false;

	return true;
}

void FrustumCulling::prepare() {
	HMatrixf projection;
	glGetFloatv(GL_PROJECTION_MATRIX,projection.getData());

	// All frustum planes face inwards.
	m_frustum[LEFT]=Plane(
		Vector3f(projection[3]+projection[0],projection[7]+projection[4],projection[11]+projection[8]),
		projection[15]+projection[12]
	);
	m_frustum[RIGHT]=Plane(
		Vector3f(projection[3]-projection[0],projection[7]-projection[4],projection[11]-projection[8]),
		projection[15]-projection[12]
	);
	m_frustum[BOTTOM]=Plane(
		Vector3f(projection[3]+projection[1],projection[7]+projection[5],projection[11]+projection[9]),
		projection[15]+projection[13]
	);
	m_frustum[TOP]=Plane(
		Vector3f(projection[3]-projection[1],projection[7]-projection[5],projection[11]-projection[9]),
		projection[15]-projection[13]
	);
	m_frustum[ZNEAR]=Plane(
		Vector3f(projection[3]+projection[2],projection[7]+projection[6],projection[11]+projection[10]),
		projection[15]+projection[14]
	);
	m_frustum[ZFAR]=Plane(
		Vector3f(projection[3]-projection[2],projection[7]-projection[6],projection[11]-projection[10]),
		projection[15]-projection[14]
	);
}

bool FrustumCulling::isVisible(const Geometry &object) const {
	AxisAlignedBoundingBox::Vertices vertices;
	object.getAABB(vertices);

	HMatrixf modelview;
	glGetFloatv(GL_MODELVIEW_MATRIX,modelview.getData());

	for (int i=7;i>=0;--i)
	  if (isVisible(modelview*vertices[i])) return true;

	return false;
}