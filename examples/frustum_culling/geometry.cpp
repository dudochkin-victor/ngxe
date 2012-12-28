#include "geometry.h"

#include <GL/glut.h>

void Geometry::getAABB(AxisAlignedBoundingBox::Vertices &vertices) const {
	Vector3f corner(m_aabb.getWidth()/2,m_aabb.getHeight()/2,m_aabb.getDepth()/2);

	vertices[0]=Vector3f(-corner.getX(),-corner.getY(), corner.getZ());
	vertices[1]=Vector3f( corner.getX(),-corner.getY(), corner.getZ());
	vertices[2]=Vector3f( corner.getX(), corner.getY(), corner.getZ());
	vertices[3]=Vector3f(-corner.getX(), corner.getY(), corner.getZ());

	vertices[4]=Vector3f( corner.getX(),-corner.getY(),-corner.getZ());
	vertices[5]=Vector3f(-corner.getX(),-corner.getY(),-corner.getZ());
	vertices[6]=Vector3f(-corner.getX(), corner.getY(),-corner.getZ());
	vertices[7]=Vector3f( corner.getX(), corner.getY(),-corner.getZ());
}

void Geometry::renderAABB() const {
  static GLuint indices[]={
		0,1,2,3, 4,5,6,7, 1,4,7,2, 5,0,3,6, 5,4,1,0, 3,2,7,6
  };

	AxisAlignedBoundingBox::Vertices vertices;
	getAABB(vertices);

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	GLint polygon_mode[2];
	glGetIntegerv(GL_POLYGON_MODE,polygon_mode);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glBegin(GL_QUADS);
		for (int i=23;i>=0;--i) glVertex3fv(vertices[indices[i]].getData());
	glEnd();

	glPolygonMode(GL_FRONT,polygon_mode[0]);
	glPolygonMode(GL_BACK,polygon_mode[1]);

	glPopAttrib();
}

void Teapot::render() const {
	GLint front_face;
	glGetIntegerv(GL_FRONT_FACE,&front_face);
	glFrontFace(GL_CW);

	// Use GLUT to render a solid Utah Teapot.
	glutSolidTeapot(1.0);

	glFrontFace(front_face);
}