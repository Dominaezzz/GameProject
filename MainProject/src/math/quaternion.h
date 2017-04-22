#pragma once

#include "vector.h"
#include <math.h>

template<typename T>
struct TQuaternion {
	union
	{
		T data[4];
		Vector<4, T> xyzw;
		struct
		{
			union
			{
				Vector<3, T> xyz;
				struct { T x, y, z; };
			};
			T w;
		};
	};

	TQuaternion() : TQuaternion(0, 0, 0, 1) {}
	TQuaternion(const T x, const T y, const T z, const T _w) : xyz(x, y, z), w(_w) {}
	TQuaternion(const Vector<3, T>& _xyz, const T _w) : xyz(_xyz), w(_w) {}
	explicit TQuaternion(const Vector<4, T>& _xyzw) : xyzw(_xyzw) {}

	T lengthSquared() const { return xyzw.lengthSquared(); }
	T length() const { return xyzw.length(); }

	TQuaternion<T>& normalize() { xyzw.normalize(); return *this; }
	TQuaternion<T> normalized() const { return TQuaternion<T>(xyzw).normalize(); }
	TQuaternion<T>& conjugate() { xyz = -xyz; return *this; }
	TQuaternion<T> conjugated() const { return TQuaternion<T>(xyzw).conjugate(); }
	TQuaternion<T>& invert() { return conjugate() /= lengthSquared(); }
	TQuaternion<T> inverted() const { return TQuaternion<T>(xyzw).invert(); }
	void toAxisAngle(Vector<3, T>& axis, T& angle) const {
		TQuaternion q = *this;
		if (abs(q.w) > 1) q.normalize();

		angle = 2.0f * acos(q.w); // angle
		float den = sqrt(1 - q.w * q.w);
		if (den > 0.0001f) {
			axis = q.xyz / den;
		} else {
			// This occurs when the angle is zero. 
			// Not a problem: just set an arbitrary normalized axis.
			axis.x = 1;
			axis.y = 0;
			axis.z = 0;
		}
	}
	static TQuaternion<T> fromAxisAngle(const Vector<3, T>& axis, const T angle)
	{
		TQuaternion<T> result(axis, cos(angle / 2));
		result.xyz.normalize();
		result.xyz *= sin(angle / 2);
		return result.normalize();
	}
	static TQuaternion<T> slerp(const TQuaternion<T>& q1, const TQuaternion<T>& q2, T blend) {
		// if either input is zero, return the other.
		if (q1.lengthSquared() == 0)
		{
			if (q2.lengthSquared() == 0)
			{
				return TQuaternion<T>(0, 0, 0, 1);
			}
			return q2;
		}
		if (q2.lengthSquared() == 0)
		{
			return q1;
		}

		T cosHalfAngle = (q1.w * q2.w) + Vector<3, T>::dot(q1.xyz, q2.xyz);

		if (cosHalfAngle >= 1 || cosHalfAngle <= -1)// angle = 0.0f, so just return one input.
		{
			return q1;
		}

		T blendA = 1 - blend;
		T blendB = blend;
		if (cosHalfAngle < 0)
		{
			cosHalfAngle = -cosHalfAngle;
			blendB = -blendB;
		}

		if (cosHalfAngle < 0.99f)// do proper slerp for big angles, do lerp if angle is really small.
		{
			T halfAngle = acos(cosHalfAngle);
			T oneOverSinHalfAngle = 1.0f / sin(halfAngle);
			blendA = sin(halfAngle * blendA) * oneOverSinHalfAngle;
			blendB = sin(halfAngle * blendB) * oneOverSinHalfAngle;
		}

		TQuaternion<T> result = (blendA * q1) + (blendB * q2);
		if (result.lengthSquared() > 0.0f) {
			return result.normalize();
		}
		return TQuaternion<T>(0, 0, 0, 1);
	}
};

template<typename T>
TQuaternion<T>& operator+=(TQuaternion<T>& lhs, const TQuaternion<T>& rhs) {
	lhs.xyzw += rhs.xyzw;
	return lhs;
}

template<typename T>
TQuaternion<T>& operator-=(TQuaternion<T>& lhs, const TQuaternion<T>& rhs) {
	lhs.xyzw -= rhs.xyzw;
	return lhs;
}

template<typename T>
TQuaternion<T>& operator*=(TQuaternion<T>& lhs, const TQuaternion<T>& rhs) {
	T tempW = (lhs.w * rhs.w) - vec::dot(lhs.xyz, rhs.xyz);
	lhs.xyz = (rhs.w * lhs.xyz) + (lhs.w * rhs.xyz) + vec::cross(lhs.xyz, rhs.xyz);
	lhs.w = tempW;
	return lhs;
}

template<typename T>
TQuaternion<T>& operator*=(TQuaternion<T>& lhs, const T rhs) {
	lhs.xyzw *= rhs;
	return lhs;
}

template<typename T>
TQuaternion<T>& operator/=(TQuaternion<T>& lhs, const T rhs) {
	lhs.xyzw /= rhs;
	return lhs;
}

template<typename T>
TQuaternion<T> operator+(TQuaternion<T> lhs, const TQuaternion<T>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T>
TQuaternion<T> operator-(TQuaternion<T> lhs, const TQuaternion<T>& rhs) {
	lhs -= rhs;
	return lhs;
}

template<typename T>
TQuaternion<T> operator*(TQuaternion<T> lhs, const TQuaternion<T>& rhs) {
	lhs *= rhs;
	return lhs;
}

template<typename T>
TQuaternion<T> operator*(TQuaternion<T> lhs, const T rhs) {
	lhs *= rhs;
	return lhs;
}

template<typename T>
TQuaternion<T> operator/(TQuaternion<T> lhs, const T rhs) {
	lhs /= rhs;
	return lhs;
}

template<typename T>
TQuaternion<T> operator-(TQuaternion<T> lhs) { 
	lhs.xyzw = -lhs.xyzw;
	return lhs;
}

template<typename T>
Vector<3, T> operator*(const TQuaternion<T>& lhs, const Vector<3, T>& rhs) {
	// Since rhs.W == 0, we can optimize quat * rhs * quat^-1 as follows:
	// rhs + 2.0 * cross(quat.xyz, cross(quat.xyz, rhs) + quat.w * rhs)

	Vector<3, T> temp = vec::cross(lhs.xyz, rhs);
	temp += rhs * lhs.w;
	temp = vec::cross(lhs.xyz, temp);
	temp += temp;

	return rhs + temp;
}

typedef TQuaternion<double> Quaterniond;
typedef TQuaternion<float> Quaternionf;
typedef TQuaternion<float> Quaternion;
