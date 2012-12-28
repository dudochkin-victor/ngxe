#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include "vector.h"

class AxisAlignedBoundingBox {
	public:

	typedef Vector3f Vertices[8];

	float getWidth() const { return m_extent.getX(); }
	void setWidth(float width) { m_extent.setX(width); }

	float getHeight() const { return m_extent.getY(); }
	void setHeight(float height) { m_extent.setY(height); }

	float getDepth() const { return m_extent.getZ(); }
	void setDepth(float depth) { m_extent.setZ(depth); }

  private:
  
	Vector3f m_extent;
};

class Geometry {
	public:

	void getAABB(AxisAlignedBoundingBox::Vertices &vertices) const;

	virtual void render() const=0;
	void renderAABB() const;

	protected:

	AxisAlignedBoundingBox m_aabb;
};

class Teapot:public Geometry {
	public:
	
	Teapot() { m_aabb.setWidth(3.5f); m_aabb.setHeight(1.7f); m_aabb.setDepth(2.0f); }

	void render() const;
};

#endif // GEOMETRY_H_INCLUDED