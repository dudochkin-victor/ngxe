#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <math.h>
#include <float.h>

#ifdef _DEBUG
  #include <iostream.h>
#endif

#pragma pack(push,1)

template<class T>
class Vector3 {
	protected:

	union {
		T m_data[3];
		struct { T m_x,m_y,m_z; };
	};

	public:

	// These are methods instead of variables in order to avoid the so called "static
	// initialization order fiasco", see http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.14.

	// Additive identity vector.
	static const Vector3& IDENTITY() { static Vector3* v=new Vector3(0,0,0); return *v; }
	// Unit vector along the x axis.
	static const Vector3& X() { static Vector3* v=new Vector3(1,0,0); return *v; }
	// Unit vector along the y axis.
	static const Vector3& Y() { static Vector3* v=new Vector3(0,1,0); return *v; }
	// Unit vector along the z axis.
	static const Vector3& Z() { static Vector3* v=new Vector3(0,0,1); return *v; }

	// Creates the additive identity vector by default.
	Vector3(T x=0,T y=0,T z=0):m_x(x),m_y(y),m_z(z) {}

	const T* const getData() const { return m_data;	}
	T* const getData() { return m_data;	}

	// Accesses the specified vector component by index.
	T& operator[](int index) { return m_data[index]; }
	// Returns the specified vector component by index (for "const" instances).
	const T& operator[](int index) const { return m_data[index]; }
	// Sets all components of this vector.
	void set(T x,T y,T z) {	setX(x); setY(y);	setZ(z); }

	// Accesses the x component of this vector.
	T& getX() { return m_x; }
	// Returns the x component of this vector.
	const T& getX() const { return m_x; }
	// Sets the x component of this vector.
	void setX(T x) { m_x=x;	}
	// Accesses the y component of this vector.
	T& getY() { return m_y; }
	// Returns the y component of this vector.
	const T& getY() const { return m_y; }
	// Sets the y component of this vector.
	void setY(T y) { m_y=y;	}
	// Accesses the z component of this vector.
	T& getZ() { return m_z; }
	// Returns the z component of this vector.
	const T& getZ() const { return m_z; }
	// Sets the z component of this vector.
	void setZ(T z) { m_z=z;	}

	// Unary sign, returns a vector unmodified (for convenience only).
	friend Vector3& operator+(const Vector3& v) { return v; }
	// Unary sign, returns the reverse of a vector.
	friend Vector3 operator-(const Vector3& v) { return Vector3(-v.m_x,-v.m_y,-v.m_z); }

  // Adds the components of two vectors. 
	friend Vector3 operator+(const Vector3& v,const Vector3& u) { return Vector3(v.m_x+u.m_x,v.m_y+u.m_y,v.m_z+u.m_z); }
	// Subtracts the components of two vectors.
  friend Vector3 operator-(const Vector3& v,const Vector3& u) { return Vector3(v.m_x-u.m_x,v.m_y-u.m_y,v.m_z-u.m_z); }
	// Adds a vector to this vector.
  const Vector3& operator+=(const Vector3& v) { m_x+=v.m_x; m_y+=v.m_y; m_z+=v.m_z; return *this; }
	// Subtracts a vector from this vector.
  const Vector3& operator-=(const Vector3& v) { m_x-=v.m_x; m_y-=v.m_y; m_z-=v.m_z; return *this; }

  // Calculates the cross product (vector product) of two vectors.
	friend Vector3 operator^(const Vector3& v,const Vector3& u) { return Vector3(v.m_y*u.m_z-v.m_z*u.m_y,v.m_z*u.m_x-v.m_x*u.m_z,v.m_x*u.m_y-v.m_y*u.m_x); }
  // Calculates the dot product (scalar product) of two vectors.
  friend T operator%(const Vector3& v,const Vector3& u) { return v.m_x*u.m_x+v.m_y*u.m_y+v.m_z*u.m_z; }

  // Multiplies a vector with a scalar.
  friend Vector3 operator*(T s,const Vector3& v) { return Vector3(s*v.m_x,s*v.m_y,s*v.m_z); }
	// Divides a vector by a scalar.
  friend Vector3 operator/(const Vector3& v,T s) { return (1/s)*v; }
	// Multiplies this vector with a scalar.
  const Vector3& operator*=(T s) { m_x*=s; m_y*=s; m_z*=s; return *this; }
	// Divides this vector by a scalar.
  const Vector3& operator/=(T s) { return (*this)*=1/s; }

  // Normalizes a vector so its length (magnitude) equals one.
  friend Vector3 operator~(const Vector3& v) { T s=v.getLength(); return (s>=FLT_EPSILON)?(v/s):v; }

  // Calculates the length (magnitude) of this vector.
  T getLength() const { return static_cast<T>(sqrt(getLengthSquared())); }
	// Calculates the squared length of this vector.
	T getLengthSquared() const { return m_x*m_x+m_y*m_y+m_z*m_z; }

  #ifdef _DEBUG
  /// Reads a vector from an input stream.
  friend istream& operator>>(istream& s,Vector3& v) { return s >> v.getX() >> v.getY() >> v.getZ(); }
  /// Writes a vector to an output stream.
  friend ostream& operator<<(ostream& s,const Vector3& v) { return s << '(' << v.getX() << ',' << v.getY() << ',' << v.getZ() << ')'; }
  #endif
};

#pragma pack(pop)

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

#endif // VECTOR_H_INCLUDED