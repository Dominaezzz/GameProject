#pragma once

#include <math.h>
#include <iostream>
#include <algorithm>

template<size_t size, typename T>
struct Vector;

namespace math
{
	template<size_t size, typename T>
	struct TVector {
		static_assert(size > 1, "Vector size should be greater than 1");

		T& operator[](const size_t i)
		{
			return static_cast<Vector<size, T>*>(this)->data[i];
		}
		const T& operator[](const size_t i) const
		{
			return static_cast<const Vector<size, T>*>(this)->data[i];
		}

		T lengthSquared() const
		{
			T sum = 0;
			for (size_t i = 0; i < size; i++)
			{
				sum += (*this)[i] * (*this)[i];
			}
			return sum;
		}

		T length() const
		{
			return sqrt(lengthSquared());
		}

		Vector<size, T>& normalize() {
			T l = length();
			for (int i = 0; i < size; ++i) (*this)[i] /= l;
			return *static_cast<Vector<size, T>*>(this);
		}

		Vector<size, T> normalized() const {
			return Vector<size, T>(*static_cast<const Vector<size, T>*>(this)).normalize();
		}
	};
}

template<size_t size, typename T>
struct Vector : public math::TVector<size, T>
{
	T data[size];

	Vector() : Vector(0) {}
	explicit Vector(T value)
	{
		for (size_t i = 0; i < size; i++)
		{
			(*this)[i] = value;
		}
	}
};

template<typename T>
struct Vector<2, T> : public math::TVector<2, T>
{
	union {
		struct
		{
			T x, y;
		};
		T data[2];
	};

	Vector() : Vector(0) {}
	explicit Vector(T value) : x(value), y(value) {}
	Vector(T _x, T _y) : x(_x), y(_y) {}
};

template<typename T>
struct Vector<3, T> : public math::TVector<3, T>
{
	union
	{
		struct
		{
			T x, y, z;
		};
		T data[3];
	};

	Vector() : Vector(0) {}
	explicit Vector(T value) : x(value), y(value), z(value) {}
	Vector(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	Vector(const Vector<2, T>& vec2, T _z) : x(vec2.x), y(vec2.y), z(_z) {}
};

template<typename T>
struct Vector<4, T> : public math::TVector<4, T> {
	union
	{
		struct
		{
			T x, y, z, w;
		};
		T data[4];
	};

	Vector() : Vector(0) {}
	explicit Vector(const T value) : x(value), y(value), z(value), w(value) {}
	Vector(const T _x, const T _y, const T _z, const T _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector(const Vector<3, T>& vec3, const T _w) : x(vec3.x), y(vec3.y), z(vec3.z), w(_w) {}
	Vector(const Vector<2, T>& vec2, const T _z, const T _w) : x(vec2.x), y(vec2.y), z(_z), w(_w) {}
};

template<size_t size, typename T>
bool operator==(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		if (lhs[i] != rhs[i])return false;
	}
	return true;
}

template<size_t size, typename T>
bool operator!=(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		if (lhs[i] != rhs[i])return true;
	}
	return false;
}

template<size_t size, typename T>
Vector<size, T>& operator+=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] += rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
Vector<size, T>& operator-=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] -= rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
Vector<size, T>& operator*=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] *= rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
Vector<size, T>& operator*=(Vector<size, T>& lhs, const T rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] *= rhs;
	}
	return lhs;
}

template<size_t size, typename T>
Vector<size, T>& operator/=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] /= rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
Vector<size, T>& operator/=(Vector<size, T>& lhs, const T rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] /= rhs;
	}
	return lhs;
}

template<size_t size, typename T>
Vector<size, T> operator+(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs += rhs;
}

template<size_t size, typename T>
Vector<size, T> operator-(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs -= rhs;
}

template<size_t size, typename T>
Vector<size, T> operator*(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs *= rhs;
}

template<size_t size, typename T>
Vector<size, T> operator*(Vector<size, T> lhs, const T rhs) {
	return lhs *= rhs;
}

template<size_t size, typename T>
Vector<size, T> operator*(const T lhs, Vector<size, T> rhs) {
	return rhs *= lhs;
}

template<size_t size, typename T>
Vector<size, T> operator/(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs /= rhs;
}

template<size_t size, typename T>
Vector<size, T> operator/(Vector<size, T> lhs, const T rhs) {
	return lhs /= rhs;
}

template<size_t size, typename T>
Vector<size, T> operator-(Vector<size, T> rhs) {
	for (int i = 0; i < size; ++i) {
		rhs[i] = -rhs[i];
	}
	return rhs;
}

template<size_t size, typename T>
std::ostream& operator<<(std::ostream& stream, const Vector<size, T>& vector) {
	stream << "Vec" << size << "(";
	for (size_t i = 0; i < size; i++)
	{
		stream << (i == 0 ? "" : ", ") << vector[i];
	}
	return stream << ")";
}

namespace vec
{
	template<size_t size, typename T>
	static T dot(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		T result = 0;
		for (size_t i = 0; i < size; i++)
		{
			result += lhs[i] * rhs[i];
		}
		return result;
	}
	template<size_t size, typename T>
	static T angle(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		T d = dot(lhs, rhs) / sqrt(lhs.lengthSquared() * rhs.lengthSquared());
		return acos(d);
	}

	template<typename T>
	static Vector<3, T> cross(const Vector<3, T>& lhs, const Vector<3, T>& rhs) {
		Vector<3, T> result;
		for (size_t i = 0; i < 3; i++)
		{
			int up = (i + 1) % 3;
			int down = (i + 2) % 3;
			result[i] = (lhs[up] * rhs[down]) - (lhs[down] * rhs[up]);
		}
		return result;
	}

	template<size_t size, typename T>
	static Vector<size, T> componentMin(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result(0);
		for (size_t i = 0; i < size; i++)
		{
			result = std::min(lhs[i], rhs[i]);
		}
		return result;
	}
	template<size_t size, typename T>
	static Vector<size, T> componentMax(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result(0);
		for (size_t i = 0; i < size; i++)
		{
			result[i] = std::max(lhs[i], rhs[i]);
		}
		return result;
	}

	template<size_t size, typename T>
	static Vector<size, T> lerp(const Vector<size, T>& lhs, const Vector<size, T>& rhs, T blend) {
		Vector<size, T> result(0);
		for (size_t i = 0; i < size; i++)
		{
			result[i] = (lhs[i] * (1 - blend)) + (rhs[i] * blend);
		}
		return result;
	}
	template<size_t size, typename T>
	static Vector<size, T> baryCentric(const Vector<size, T>& a, const Vector<size, T>& b, const Vector<size, T>& c, T u, T v) {
		return a + u * (b - a) + v * (c - a);
	}
}

typedef Vector<2, int> Vector2i;
typedef Vector<2, float> Vector2;
typedef Vector<2, float> Vector2f;
typedef Vector<2, double> Vector2d;

typedef Vector<3, int> Vector3i;
typedef Vector<3, float> Vector3;
typedef Vector<3, float> Vector3f;
typedef Vector<3, double> Vector3d;

typedef Vector<4, int> Vector4i;
typedef Vector<4, float> Vector4;
typedef Vector<4, float> Vector4f;
typedef Vector<4, double> Vector4d;

// template <typename T> const Vector<T> Vector<T>::right(1, 0, 0);
// template <typename T> const Vector<T> Vector<T>::up(0, 1, 0);
// template <typename T> const Vector<T> Vector<T>::forward(0, 0, 1);
