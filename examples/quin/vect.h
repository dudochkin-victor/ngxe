///////////////////////////////////////////////////////////////////////////////
// vect.h: interface for a generic vector class.
// Operators defined are: + (vector addition)
//                       += (vector addition)
//                        - (vector subtraction)
//                        - (unary negation)
//                       -= (vector subtraction)
//                        = (vector assignment)
//                        % (dot product)
//                        * (cross product)
//                        & (elementwise product)
//                       &= (elementwise product)
//                        / (elementwise divide)
//                       /= (elementwise divide)
//                        * (scalar multiply)
//                       *= (scalar multiply)
//                        / (scalar divide)
//                       /= (scalar divide)
//                normalize (sets length to 1)
//                magnitude (returns length)
//                     pack (returns a packed int colour value)

// Author: Quin Pendragon (dragon@graduate.uwa.edu.au)
// Date:   2002-11-12

#ifndef vect_h
#define vect_h

#include <math.h>

class vect
{
public:
	union { float x; float r; };
	union { float y; float g; };
	union { float z; float b; };

	vect() : x(0), y(0), z(0) { }
	vect(float a, float b, float c)
		: x(a), y(b), z(c) { }
	vect(const vect& v) { *this = v; }
	vect(float f) : x(f), y(f), z(f) { }

	vect operator+(const vect& other) const {return vect(x+other.x, y+other.y, z+other.z);}
	vect operator-(const vect& other) const {return vect(x-other.x, y-other.y, z-other.z);}
	vect operator&(const vect& other) const {return vect(x*other.x, y*other.y, z*other.z);}
	vect operator/(const vect& other) const {return vect(x/other.x, y/other.y, z/other.z);}
	vect operator*(const vect& other) const {
		return vect(y*other.z-z*other.y,
			z*other.x-x*other.z,
			x*other.y-y*other.x);
	}

	friend vect operator-(const vect& other) { return vect(-other.x, -other.y, -other.z); }

	vect& operator+=(const vect& other) { x += other.x;
		y += other.y; z += other.z; return *this; }
	vect& operator-=(const vect& other) { x -= other.x;
		y -= other.y; z -= other.z; return *this; }
	vect& operator&=(const vect& other) { x *= other.x;
		y *= other.y; z *= other.z; return *this; }
	vect& operator/=(const vect& other) { x /= other.x;
		y /= other.y; z /= other.z; return *this; }

	vect& operator=(const vect& other) { x = other.x;
		y = other.y; z = other.z; return *this; }

	float operator%(vect& other) const {return x*other.x + y*other.y + z*other.z;}

	vect operator*(const float val) const { return vect(x*val, y*val,z*val); }
	vect& operator*=(const float val) { x *= val;
		y *= val; z *= val; return *this; }
	vect operator/(const float val) const { return vect(x/val, y/val, z/val); }
	vect& operator/=(const float val) { x /= val;
		y /= val; z /= val; return *this; }

	vect& normalize() { float f = 1.0f / magnitude();
		x *= f; y *= f; z *= f; return *this; }
	float magnitude() const { return (float)sqrt(x*x+y*y+z*z); }
	void clip() {
		if (x < 0.0f) x = 0.0f; if (x > 1.0f) x = 1.0f;
		if (y < 0.0f) y = 0.0f; if (y > 1.0f) y = 1.0f;
		if (z < 0.0f) z = 0.0f; if (z > 1.0f) z = 1.0f;
	}
	int pack() {
		float rc = (r > 0.0f) ? r : 0.0f; if (rc > 1.0f) rc = 1.0f;
		float gc = (g > 0.0f) ? g : 0.0f; if (gc > 1.0f) gc = 1.0f;
		float bc = (b > 0.0f) ? b : 0.0f; if (bc > 1.0f) bc = 1.0f;
		return 0xFF000000|((int)(rc*0xFF)<<16)|((int)(gc*0xFF)<<8)|(int)(bc*0xFF);
	}

	friend vect operator*(float f, vect& v) { return v * f; }
};

typedef vect colour;

#endif // vect_h
