#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "vector.h"

#ifdef _DEBUG
#include <iostream.h>
#endif

#pragma pack(push,1)

template<class T>
class HMatrix {
protected:

	union {
		T m_data[16];
		struct {
			Vector3<T> m_n;
			T m_nw;
			Vector3<T> m_o;
			T m_ow;
			Vector3<T> m_a;
			T m_aw;
			Vector3<T> m_p;
			T m_pw;
		};
	};

public:

	// Multiplicative identity matrix. This is a method instead of a variable in order to avoid the so called
	// "static initialization order fiasco", see http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.14.
	static const HMatrix& IDENTITY() {
		static HMatrix* m = new HMatrix(Vector3<T>::X(), Vector3<T>::Y(),
				Vector3<T>::Z(), Vector3<T>::IDENTITY());
		return *m;
	}

	// Creates the multiplicative identity matrix by default.
	HMatrix(const Vector3<T>& n = Vector3<T>::X(), const Vector3<T>& o =
			Vector3<T>::Y(), const Vector3<T>& a = Vector3<T>::Z(),
			const Vector3<T>& p = Vector3<T>::IDENTITY()) :
			m_n(n), m_o(o), m_a(a), m_p(p), m_nw(0), m_ow(0), m_aw(0), m_pw(1) {
	}

	const T* const getData() const {
		return m_data;
	}
	T* const getData() {
		return m_data;
	}

	// Accesses the specified matrix component by index.
	T& operator[](int index) {
		return m_data[index];
	}
	// Returns the specified matrix component by index (for "const" instances).
	const T& operator[](int index) const {
		return m_data[index];
	}
	// Sets all components of this matrix.
	void set(const Vector3<T>& n, const Vector3<T>& o, const Vector3<T>& a,
			const Vector3<T>& p) {
		setN(n);
		setO(o);
		setA(a);
		setP(p);
	}

	// Accesses the "normal" vector component of this matrix.
	Vector3<T>& getN() {
		return m_n;
	}
	// Returns the "normal" vector component of this matrix.
	const Vector3<T>& getN() const {
		return m_n;
	}
	// Sets the "normal" vector component of this matrix.
	void setN(const Vector3<T>& n) {
		m_n = n;
	}
	// Accesses the "open" vector component of this matrix.
	Vector3<T>& getO() {
		return m_o;
	}
	// Returns the "open" vector component of this matrix.
	const Vector3<T>& getO() const {
		return m_o;
	}
	// Sets the "open" vector component of this matrix.
	void setO(const Vector3<T>& o) {
		m_o = o;
	}
	// Accesses the "approach" vector component of this matrix.
	Vector3<T>& getA() {
		return m_a;
	}
	// Returns the "approach" vector component of this matrix.
	const Vector3<T>& getA() const {
		return m_a;
	}
	// Sets the "approach" vector component of this matrix.
	void setA(const Vector3<T>& a) {
		m_a = a;
	}
	// Accesses the "position" vector component (translation part) of this matrix.
	Vector3<T>& getP() {
		return m_p;
	}
	// Returns the "position" vector component (translation part) of this matrix.
	const Vector3<T>& getP() const {
		return m_p;
	}
	// Sets the "position" vector component (translation part) of this matrix.
	void setP(const Vector3<T>& p) {
		m_p = p;
	}

	// Unary sign, returns a matrix unmodified.
	friend HMatrix& operator+(const HMatrix& m) {
		return m;
	}
	// Unary sign, returns the reverse of a matrix.
	friend HMatrix operator-(const HMatrix& m) {
		return HMatrix(-m.m_n, -m.m_o, -m.m_a, -m.m_p);
	}

	// Adds the components of two matrices.
	friend HMatrix operator+(const HMatrix& m, const HMatrix& l) {
		return HMatrix(m.m_n + l.m_n, m.m_o + l.m_o, m.m_a + l.m_a,
				m.m_p + l.m_p);
	}
	// Subtracts the components of two matrices.
	friend HMatrix operator-(const HMatrix& m, const HMatrix& l) {
		return HMatrix(m.m_n - l.m_n, m.m_o - l.m_o, m.m_a - l.m_a,
				m.m_p - l.m_p);
	}
	// Adds a matrix to this  matrix.
	const HMatrix& operator+=(const HMatrix& m) {
		m_n += m.m_n;
		m_o += m.m_o;
		m_a += m.m_a;
		m_p += m.m_p;
		return *this;
	}
	// Subtracts a matrix from this  matrix.
	const HMatrix& operator-=(const HMatrix& m) {
		m_n -= m.m_n;
		m_o -= m.m_o;
		m_a -= m.m_a;
		m_p -= m.m_p;
		return *this;
	}

	// Multiplies a matrix with another matrix.
#ifdef _MSC_VER
	friend HMatrix operator*(const HMatrix& m,const HMatrix& l);
#else
	friend HMatrix operator*<>(const HMatrix& m, const HMatrix& l);
#endif
	// Multiplies this matrix with another matrix.
	const HMatrix& operator*=(const HMatrix& m) {
		return *this = (*this) * m;
	}

	// Multiplies a matrix with a vector.
#ifdef _MSC_VER
	friend Vector3<T> operator*(const HMatrix& m,const Vector3<T>& v);
#else
	friend Vector3<T> operator*<>(const HMatrix& m, const Vector3<T>& v);
#endif

	// Orthonormalizes a matrix so its vectors are perpendicular to each other and equal one in length.
#ifdef _MSC_VER
	friend HMatrix operator~(const HMatrix& m);
#else
	friend HMatrix operator~<>(const HMatrix& m);
#endif

	// Returns the inverse of this matrix.
	HMatrix operator!() const;

	// Creates a rotation matrix for the given (normalized) axis and angle.
	static HMatrix createRotation(const Vector3<T>& axis, double angle);
	// Creates a rotation matrix about the x-axis and the given angle.
	static HMatrix createRotationX(double angle);
	// Creates a rotation matrix about the y-axis and the given angle.
	static HMatrix createRotationY(double angle);
	// Creates a rotation matrix about the z-axis and the given angle.
	static HMatrix createRotationZ(double angle);

	// Sets the position vector to match a rotation around an arbitrary center.
	void setRotationCenter(const Vector3<T>& center);

	// Creates a translation matrix for the given direction and distance.
	static HMatrix createTranslation(const Vector3<T>& direction,
			double distance) {
		return HMatrix(Vector3<T>::X(), Vector3<T>::Y(), Vector3<T>::Z(),
				distance * (~direction));
	}
	// Creates a translation matrix along the x-axis and the given distance.
	static HMatrix createTranslationX(T distance) {
		return createTranslation(Vector3<T>::X(), distance);
	}
	// Creates a translation matrix along the y-axis and the given distance.
	static HMatrix createTranslationY(T distance) {
		return createTranslation(Vector3<T>::Y(), distance);
	}
	// Creates a translation matrix along the z-axis and the given distance.
	static HMatrix createTranslationZ(T distance) {
		return createTranslation(Vector3<T>::Z(), distance);
	}

#ifdef _DEBUG
	/// Reads a matrix from an input stream.
	friend istream& operator>>(istream& s,HMatrix& m) {return s >> m.getN() >> m.getO() >> m.getA() >> m.getP();}
	/// Writes a matrix to an output stream.
	friend ostream& operator<<(ostream& s,const HMatrix& m) {return s << '(' << m.getN() << ',' << m.getO() << ',' << m.getA() << ',' << m.getP() << ')';}
#endif
};

#pragma pack(pop)

typedef HMatrix<float> HMatrixf;
typedef HMatrix<double> HMatrixd;

template<class T>
HMatrix<T> operator*(const HMatrix<T>& m, const HMatrix<T>& l) {
	Vector3<T> n, o, a, p;

	for (int row = 2; row >= 0; --row) {
		n[row] = m[row] * l[0] + m[row + 4] * l[1] + m[row + 8] * l[2];
		o[row] = m[row] * l[4] + m[row + 4] * l[5] + m[row + 8] * l[6];
		a[row] = m[row] * l[8] + m[row + 4] * l[9] + m[row + 8] * l[10];
		p[row] = m[row] * l[12] + m[row + 4] * l[13] + m[row + 8] * l[14]
				+ m[row + 12];
	}

	return HMatrix<T>(n, o, a, p);
}

template<class T>
Vector3<T> operator*(const HMatrix<T>& m, const Vector3<T>& v) {
	return Vector3<T>(
			m[0] * v.getX() + m[4] * v.getY() + m[8] * v.getZ() + m[12],
			m[1] * v.getX() + m[5] * v.getY() + m[9] * v.getZ() + m[13],
			m[2] * v.getX() + m[6] * v.getY() + m[10] * v.getZ() + m[14]);
}

template<class T>
HMatrix<T> operator~(const HMatrix<T>& m) {
	Vector3<T> n, a;

	n = ~m.getN();
	a = ~(m.getO() ^ n);

	return HMatrix<T>(n, ~(a ^ n), a, m.getP());
}

template<class T>
HMatrix<T> HMatrix<T>::operator!() const {
	return HMatrix(Vector3<T>(getN().getX(), getO().getX(), getA().getX()),
			Vector3<T>(getN().getY(), getO().getY(), getA().getY()),
			Vector3<T>(getN().getZ(), getO().getZ(), getA().getZ()),
			Vector3<T>(-getP() % getN(), -getP() % getO(), -getP() % getA()));
}

template<class T>
HMatrix<T> HMatrix<T>::createRotation(const Vector3<T>& axis, double angle) {
	T s = static_cast<T>(sin(angle));
	T c = static_cast<T>(1 - cos(angle));

	T xs = axis.getX() * s;
	T ys = axis.getY() * s;
	T zs = axis.getZ() * s;

	T xc = axis.getX() * c;
	T yc = axis.getY() * c;
	T zc = axis.getZ() * c;

	return HMatrix(
			Vector3<T>(1 - c + axis.getX() * xc, zs + axis.getY() * xc,
					-ys + axis.getZ() * xc),
			Vector3<T>(-zs + axis.getX() * yc, 1 - c + axis.getY() * yc,
					xs + axis.getZ() * yc),
			Vector3<T>(ys + axis.getX() * zc, -xs + axis.getY() * zc,
					1 - c + axis.getZ() * zc));
}

template<class T>
HMatrix<T> HMatrix<T>::createRotationX(double angle) {
	T s = static_cast<T>(sin(angle));
	T c = static_cast<T>(cos(angle));

	return HMatrix(Vector3<T>::X(), Vector3<T>(0, c, s), Vector3<T>(0, -s, c));
}

template<class T>
HMatrix<T> HMatrix<T>::createRotationY(double angle) {
	T s = static_cast<T>(sin(angle));
	T c = static_cast<T>(cos(angle));

	return HMatrix(Vector3<T>(c, 0, -s), Vector3<T>::Y(), Vector3<T>(s, 0, c));
}

template<class T>
HMatrix<T> HMatrix<T>::createRotationZ(double angle) {
	T s = static_cast<T>(sin(angle));
	T c = static_cast<T>(cos(angle));

	return HMatrix(Vector3<T>(c, s, 0), Vector3<T>(-s, c, 0), Vector3<T>::Z());
}

template<class T>
void HMatrix<T>::setRotationCenter(const Vector3<T>& center) {
	setP(
			Vector3f(
					center.getX() - center.getX() * getN().getX()
							- center.getY() * getO().getX()
							- center.getZ() * getA().getX(),
					-center.getX() * getN().getY() + center.getY()
							- center.getY() * getO().getY()
							- center.getZ() * getA().getY(),
					-center.getX() * getN().getZ()
							- center.getY() * getO().getZ() + center.getZ()
							- center.getZ() * getA().getZ()));
}

#endif // MATRIX_H_INCLUDED
