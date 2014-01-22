#pragma once
#include <math.h>

inline double squared (double input) {return input * input;}
inline double asRadians (double degrees) {return degrees * (3.14159 / 180.0);}
inline double asDegrees (double radians) {return radians * (180.0 / 3.14159);}
inline double absolute (double number) {return fabs (number);} //Let the compiler pick
inline long absolute (long number) {return abs (number);} //the right absolute value function.
inline float absolute (float number) {return fabs (number);}

inline float epsilon () {return 1.0e-5f;};
inline float squaredEpsilon () {return 1.0e-10f;};
inline float inverseEpsilon () {return 1.0f / epsilon ();}

class Vector
{
public:
	float x, y, z;

	Vector () {x = 0.0; y = 0.0; z = 0.0;}
	Vector (float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
	Vector (const Vector &point) {x = point.x; y = point.y; z = point.z;}
	~Vector () {}
	
	inline void zero () {x = y = z = 0.0;}
	inline bool isZero () const {return fabs (x) < epsilon () && fabs (y) < epsilon () && fabs (z) < epsilon ();}
	inline Vector operator * (const float &d) const {return Vector (x*d, y*d, z*d);}
    inline void operator *= (const float &d) {x*=d; y*=d; z*=d;}
	inline Vector operator / (const float &d) const {float inverse = 1.0f/d; return Vector (x*inverse, y*inverse, z*inverse);}
    inline void operator /= (const float &d) {x/=d; y/=d; z/=d;}
	inline Vector operator + (const Vector &p) const {return Vector (x+p.x, y+p.y, z+p.z);}
    inline void operator += (const Vector &p) {x+=p.x; y+=p.y; z+=p.z;}
    inline void operator += (const float &d) {x+=d; y+=d; z+=d;}
	inline Vector operator - (const Vector &p) const {return Vector (x-p.x, y-p.y, z-p.z);}
    inline void operator -= (const Vector &p) {x-=p.x; y-=p.y; z-=p.z;}
    inline void operator -= (const float &d) {x-=d; y-=d; z-=d;}
	inline Vector operator - () const {return Vector (-x, -y, -z);}; //unary
	inline bool operator == (const Vector &p) { return x == p.x && y == p.y && z == p.z;}
	inline float squaredLength () {return x*x + y*y + z*z;}
	inline float length () {return sqrt (squaredLength ());}
	inline float dot (const Vector &p) const {return x*p.x + y*p.y + z*p.z;}
	inline NxVec3 asNxVec3() const { return NxVec3(x, y, z);}
	inline D3DXVECTOR3 asD3DXVECTOR3() const { return D3DXVECTOR3(x, y, z);}

	inline Vector cross (const Vector &v) const {
		//Vector cross product (this cross v).
		return Vector (y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
	}

	inline Vector pointCross (Vector &p2, Vector &p3) {
		//Vector cross product assuming p1,p2,p3 are counter-clockwise.
		//Create vectors from p1 (this) to p2, and p1 to p3.
		Vector vector1 (p2.x - x, p2.y - y, p2.z - z);
		Vector vector2 (p3.x - x, p3.y - y, p3.z - z);
		return vector1.cross (vector2);
	}

	inline void normalize (float &length) {//destructively and ALSO returns length
		length = sqrt (squaredLength ());
		if (absolute (length) > epsilon ()) {
			float oneOverLength = 1.0f / length;
			x *= oneOverLength; y *= oneOverLength; z *= oneOverLength;
		}
	}
	inline void normalize () {float length; normalize (length);} //destructively
	inline Vector normalized () const {Vector result = *this; result.normalize (); return result;}
	inline Vector normalized (float &length) const {Vector result = *this; result.normalize (length); return result;}

	inline float squaredDistanceTo (Vector &point) {Vector difference = point - *this; return difference.dot (difference);}

	
};

