// Copyright(c) 1999-2018 ASL author
// Licensed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef ASL_VECTOR2_H
#define ASL_VECTOR2_H

#include <asl/defs.h>
#include <math.h>

namespace asl {

/**
Vec2 represents a vector in 2D space.

This class allows operating with vectors as with primitive types via operators.

~~~
Vec2 a (10, 10);
Vec2 b (-5, 1.5);
Vec2 c = -a.rotate(PI/4) + 2.0 * (b - a).normalized();
~~~

The angle between two vectors can be computed with:
~~~
float angle = acos(a/!a * b/!b);
~~~
*/

class Vec2
{
 public:
	/** Constructs a vector with the given (x, y) coordinates */
	Vec2(float X, float Y) {x=X; y=Y;}
	Vec2(const Vec2& v) {x=v.x; y=v.y;}
	Vec2() {}
	/** Sets the x, y components of this vector */
	void set(float X, float Y) { x=X; y=Y; }
	void get(float& X, float& Y) const { X=x; Y=y; }
	/** Returns a vector constructed from polar coordinates */
	static Vec2 polar(float r, float a) {return Vec2(r*cos(a), r*sin(a));}
	/** Returns this vector rotated 90� counterclockwise. */
	Vec2 perpend() const {return Vec2(-y, x);}
	/** Returns this vector rotated an angle `a`. */
	Vec2 rotate(float a) const {float s=sin(a), c=cos(a); return Vec2(c*x-s*y, s*x+c*y);}
	/** Returns this vector normalized. */
	Vec2 normalized() const {return (*this)/length();}
	/** Returns the angle between this vector and the +X axis in the [-PI, PI] range */
	float angle()     const {return (float) atan2(y, x);}
	/** Returns the length of this vector */
	float operator!() const {return sqrt(x*x+y*y);}
	/** Returns the length of this vector */
	float length()    const {return sqrt(x*x+y*y);}
	/** Returns the length of this vector squared */
	float length2()   const {return x*x+y*y;}
	float norm1()     const {return (float)(fabs(x)+fabs(y));}
	float normInf()   const {return (float)max(fabs((float)x), fabs((float)y));}

	/** Assings `b` to this vector */
	void operator=(const Vec2& b) {x=b.x; y=b.y;}
	/** Returns this vector plus `b` */
	Vec2 operator+(const Vec2& b) const {return Vec2(x+b.x, y+b.y);}
	/** Returns this vector minus `b` */
	Vec2 operator-(const Vec2& b) const {return Vec2(x-b.x, y-b.y);}
	/** Returns the dot product of this vector and `b` */
	float operator*(const Vec2& b) const {return x*b.x+y*b.y;}
	/** Returns this vector multiplied by scalar `r` */
	Vec2 operator*(float r) const {return Vec2(x*r, y*r);}
	/** Returns vector `b` multiplied by scalar `r` */
	friend Vec2 operator*(float r, const Vec2& b) {return Vec2(b.x*r, b.y*r);}
	/** Returns the z-component of the cross product of this vector and `b` */
	float operator^(const Vec2& b) const {return x*b.y-y*b.x;}
	/** Returns this vector divided by scalar `r` */
	Vec2 operator/(float r) const {float q=1/r; return Vec2(x*q, y*q);}
	/** Compares this vector and `b` (warning: test equality of floats with care) */
	bool operator==(const Vec2& b) const {return x==b.x && y==b.y;}
	/** Compares this vector and `b` (warning: test equality of floats with care) */
	bool operator!=(const Vec2& b) const {return x!=b.x || y!=b.y;}
	/** Adds `b` to this vector */
	void operator+=(const Vec2& b) {x += b.x; y += b.y;}
	/** Subtracts `b` from this vector */
	void operator-=(const Vec2& b) {x -= b.x; y -= b.y;}
	/** Multiplies this vector by scalar `r` */
	void operator*=(float r) {x *= r; y *= r;}
	/** Divides this vector by scalar `r` */
	void operator/=(float r) {x /= r; y /= r;}
	/** Returns this vector negated */
	Vec2 operator-() const {return Vec2(-x, -y);}
 public:
	float x, y;
};

inline int compare(const Vec2& a, const Vec2& b)
{
	if(a.x < b.x) return -1;
	else if(a.x == b.x && a.y < b.y) return -1;
	else if(a.x == b.x && a.y == b.y) return 0;
	else return 1;
}

inline Vec2 min(const Vec2& a, const Vec2& b)
{
	return Vec2(min(a.x, b.x), min(a.y, b.y));
}

inline Vec2 max(const Vec2& a, const Vec2& b)
{
	return Vec2(max(a.x, b.x), max(a.y, b.y));
}

inline const Vec2 min(const Vec2& a, const Vec2& b, const Vec2& c)
{
	return min(min(a, b), c);
}

inline Vec2 max(const Vec2& a, const Vec2& b, const Vec2& c)
{
	return max(max(a, b), c);
}

}
#endif
