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

		constexpr T& operator[](const size_t i)
		{
			return static_cast<Vector<size, T>*>(this)->data[i];
		}
		constexpr const T& operator[](const size_t i) const
		{
			return static_cast<const Vector<size, T>*>(this)->data[i];
		}

		constexpr T lengthSquared() const
		{
			T sum = 0;
			for (size_t i = 0; i < size; i++)
			{
				sum += (*this)[i] * (*this)[i];
			}
			return sum;
		}

		constexpr T length() const
		{
			return sqrt(lengthSquared());
		}

		constexpr Vector<size, T>& normalize() {
			T l = length();
			for (int i = 0; i < size; ++i) (*this)[i] /= l;
			return *static_cast<Vector<size, T>*>(this);
		}

		constexpr Vector<size, T> normalized() const {
			return Vector<size, T>(*static_cast<const Vector<size, T>*>(this)).normalize();
		}
	};
}

template<size_t size, typename T>
struct Vector : public math::TVector<size, T>
{
	T data[size];

	constexpr Vector() : Vector(0) {}
	constexpr explicit Vector(T value)
	{
		for (size_t i = 0; i < size; i++)
		{
			(*this)[i] = value;
		}
	}
};

template<typename T, size_t c1, size_t c2>
struct Swizle_2
{
	T vec[(c1 > c2 ? c1 : c2) + 1];

	Vector<2, T> operator=(const Vector<2, T>& rhs)
	{
		vec[c1] = rhs[0];
		vec[c2] = rhs[0];
		return rhs;
	}
	operator Vector<2, T>() const
	{
		return Vector<2, T>(vec[c1], vec[c2]);
	}
};

template<typename T, size_t c1, size_t c2, size_t c3>
struct Swizle_3
{
	T vec[(c1 > c2 ? c1 : (c2 > c3 ? c2 : c3)) + 1];

	Vector<3, T> operator=(const Vector<3, T>& rhs)
	{
		vec[c1] = rhs[0];
		vec[c2] = rhs[1];
		vec[c3] = rhs[2];
		return rhs;
	}
	operator Vector<3, T>() const
	{
		return Vector<3, T>(vec[c1], vec[c2], vec[c3]);
	}
};

template<typename T, size_t c1, size_t c2, size_t c3, size_t c4>
struct Swizle_4
{
	T vec[(c1 > c2 ? c1 : (c2 > c3 ? c2 : (c3 > c4 ? c3 : c4))) + 1];

	Vector<4, T> operator=(const Vector<4, T>& rhs)
	{
		vec[c1] = rhs[0];
		vec[c2] = rhs[1];
		vec[c3] = rhs[2];
		vec[c4] = rhs[3];
		return rhs;
	}
	operator Vector<4, T>() const
	{
		return Vector<4, T>(vec[c1], vec[c2], vec[c3], vec[c4]);
	}
};

template<typename T>
struct Vector<2, T> : public math::TVector<2, T>
{
	union
	{
		struct
		{
			T x, y;
		};
		T data[2];

		// 2D swizzles
		union
		{
			Swizle_2<T, 0, 0> xx;
			Swizle_2<T, 0, 1> xy;
			Swizle_2<T, 1, 0> yx;
			Swizle_2<T, 1, 1> yy;
		};

		// 3D swizzles
		union
		{
			Swizle_3<T, 0, 0, 0> xxx;
			Swizle_3<T, 0, 0, 1> xxy;
			Swizle_3<T, 0, 1, 0> xyx;
			Swizle_3<T, 0, 1, 1> xyy;
			Swizle_3<T, 1, 0, 0> yxx;
			Swizle_3<T, 1, 0, 1> yxy;
			Swizle_3<T, 1, 1, 0> yyx;
			Swizle_3<T, 1, 1, 1> yyy;
		};

		// 4D swizzles
		union
		{
			Swizle_4<T, 0, 0, 0, 0> xxxx;
			Swizle_4<T, 0, 0, 0, 1> xxxy;
			Swizle_4<T, 0, 0, 1, 0> xxyx;
			Swizle_4<T, 0, 0, 1, 1> xxyy;
			Swizle_4<T, 0, 1, 0, 0> xyxx;
			Swizle_4<T, 0, 1, 0, 1> xyxy;
			Swizle_4<T, 0, 1, 1, 0> xyyx;
			Swizle_4<T, 0, 1, 1, 1> xyyy;

			Swizle_4<T, 1, 0, 0, 0> yxxx;
			Swizle_4<T, 1, 0, 0, 1> yxxy;
			Swizle_4<T, 1, 0, 1, 0> yxyx;
			Swizle_4<T, 1, 0, 1, 1> yxyy;
			Swizle_4<T, 1, 1, 0, 0> yyxx;
			Swizle_4<T, 1, 1, 0, 1> yyxy;
			Swizle_4<T, 1, 1, 1, 0> yyyx;
			Swizle_4<T, 1, 1, 1, 1> yyyy;
		};
	};

	constexpr Vector() : Vector(0) {}
	constexpr explicit Vector(T value) : x(value), y(value) {}
	constexpr Vector(T _x, T _y) : x(_x), y(_y) {}
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

		// 2D swizzles
		union
		{
			Swizle_2<T, 0, 0> xx;
			Swizle_2<T, 0, 1> xy;
			Swizle_2<T, 0, 2> xz;
			Swizle_2<T, 1, 0> yx;
			Swizle_2<T, 1, 1> yy;
			Swizle_2<T, 1, 2> yz;
			Swizle_2<T, 2, 0> zx;
			Swizle_2<T, 2, 1> zy;
			Swizle_2<T, 2, 2> zz;
		};

		// 3D swizzles
		union
		{
			Swizle_3<T, 0, 0, 0> xxx;
			Swizle_3<T, 0, 0, 1> xxy;
			Swizle_3<T, 0, 0, 2> xxz;
			Swizle_3<T, 0, 1, 0> xyx;
			Swizle_3<T, 0, 1, 1> xyy;
			Swizle_3<T, 0, 1, 2> xyz;
			Swizle_3<T, 0, 2, 0> xzx;
			Swizle_3<T, 0, 2, 1> xzy;
			Swizle_3<T, 0, 2, 2> xzz;

			Swizle_3<T, 1, 0, 0> yxx;
			Swizle_3<T, 1, 0, 1> yxy;
			Swizle_3<T, 1, 0, 2> yxz;
			Swizle_3<T, 1, 1, 0> yyx;
			Swizle_3<T, 1, 1, 1> yyy;
			Swizle_3<T, 1, 1, 2> yyz;
			Swizle_3<T, 1, 2, 0> yzx;
			Swizle_3<T, 1, 2, 1> yzy;
			Swizle_3<T, 1, 2, 2> yzz;

			Swizle_3<T, 2, 0, 0> zxx;
			Swizle_3<T, 2, 0, 1> zxy;
			Swizle_3<T, 2, 0, 2> zxz;
			Swizle_3<T, 2, 1, 0> zyx;
			Swizle_3<T, 2, 1, 1> zyy;
			Swizle_3<T, 2, 1, 2> zyz;
			Swizle_3<T, 2, 2, 0> zzx;
			Swizle_3<T, 2, 2, 1> zzy;
			Swizle_3<T, 2, 2, 2> zzz;
		};

		// 4D swizzles
		union
		{
			Swizle_4<T, 0, 0, 0, 0> xxxx;
			Swizle_4<T, 0, 0, 0, 1> xxxy;
			Swizle_4<T, 0, 0, 0, 2> xxxz;
			Swizle_4<T, 0, 0, 1, 0> xxyx;
			Swizle_4<T, 0, 0, 1, 1> xxyy;
			Swizle_4<T, 0, 0, 1, 2> xxyz;
			Swizle_4<T, 0, 0, 2, 0> xxzx;
			Swizle_4<T, 0, 0, 2, 1> xxzy;
			Swizle_4<T, 0, 0, 2, 2> xxzz;
			Swizle_4<T, 0, 1, 0, 0> xyxx;
			Swizle_4<T, 0, 1, 0, 1> xyxy;
			Swizle_4<T, 0, 1, 0, 2> xyxz;
			Swizle_4<T, 0, 1, 1, 0> xyyx;
			Swizle_4<T, 0, 1, 1, 1> xyyy;
			Swizle_4<T, 0, 1, 1, 2> xyyz;
			Swizle_4<T, 0, 1, 2, 0> xyzx;
			Swizle_4<T, 0, 1, 2, 1> xyzy;
			Swizle_4<T, 0, 1, 2, 2> xyzz;
			Swizle_4<T, 0, 2, 0, 0> xzxx;
			Swizle_4<T, 0, 2, 0, 1> xzxy;
			Swizle_4<T, 0, 2, 0, 2> xzxz;
			Swizle_4<T, 0, 2, 1, 0> xzyx;
			Swizle_4<T, 0, 2, 1, 1> xzyy;
			Swizle_4<T, 0, 2, 1, 2> xzyz;
			Swizle_4<T, 0, 2, 2, 0> xzzx;
			Swizle_4<T, 0, 2, 2, 1> xzzy;
			Swizle_4<T, 0, 2, 2, 2> xzzz;


			Swizle_4<T, 1, 0, 0, 0> yxxx;
			Swizle_4<T, 1, 0, 0, 1> yxxy;
			Swizle_4<T, 1, 0, 0, 2> yxxz;
			Swizle_4<T, 1, 0, 1, 0> yxyx;
			Swizle_4<T, 1, 0, 1, 1> yxyy;
			Swizle_4<T, 1, 0, 1, 2> yxyz;
			Swizle_4<T, 1, 0, 2, 0> yxzx;
			Swizle_4<T, 1, 0, 2, 1> yxzy;
			Swizle_4<T, 1, 0, 2, 2> yxzz;
			Swizle_4<T, 1, 1, 0, 0> yyxx;
			Swizle_4<T, 1, 1, 0, 1> yyxy;
			Swizle_4<T, 1, 1, 0, 2> yyxz;
			Swizle_4<T, 1, 1, 1, 0> yyyx;
			Swizle_4<T, 1, 1, 1, 1> yyyy;
			Swizle_4<T, 1, 1, 1, 2> yyyz;
			Swizle_4<T, 1, 1, 2, 0> yyzx;
			Swizle_4<T, 1, 1, 2, 1> yyzy;
			Swizle_4<T, 1, 1, 2, 2> yyzz;
			Swizle_4<T, 1, 2, 0, 0> yzxx;
			Swizle_4<T, 1, 2, 0, 1> yzxy;
			Swizle_4<T, 1, 2, 0, 2> yzxz;
			Swizle_4<T, 1, 2, 1, 0> yzyx;
			Swizle_4<T, 1, 2, 1, 1> yzyy;
			Swizle_4<T, 1, 2, 1, 2> yzyz;
			Swizle_4<T, 1, 2, 2, 0> yzzx;
			Swizle_4<T, 1, 2, 2, 1> yzzy;
			Swizle_4<T, 1, 2, 2, 2> yzzz;


			Swizle_4<T, 2, 0, 0, 0> zxxx;
			Swizle_4<T, 2, 0, 0, 1> zxxy;
			Swizle_4<T, 2, 0, 0, 2> zxxz;
			Swizle_4<T, 2, 0, 1, 0> zxyx;
			Swizle_4<T, 2, 0, 1, 1> zxyy;
			Swizle_4<T, 2, 0, 1, 2> zxyz;
			Swizle_4<T, 2, 0, 2, 0> zxzx;
			Swizle_4<T, 2, 0, 2, 1> zxzy;
			Swizle_4<T, 2, 0, 2, 2> zxzz;
			Swizle_4<T, 2, 1, 0, 0> zyxx;
			Swizle_4<T, 2, 1, 0, 1> zyxy;
			Swizle_4<T, 2, 1, 0, 2> zyxz;
			Swizle_4<T, 2, 1, 1, 0> zyyx;
			Swizle_4<T, 2, 1, 1, 1> zyyy;
			Swizle_4<T, 2, 1, 1, 2> zyyz;
			Swizle_4<T, 2, 1, 2, 0> zyzx;
			Swizle_4<T, 2, 1, 2, 1> zyzy;
			Swizle_4<T, 2, 1, 2, 2> zyzz;
			Swizle_4<T, 2, 2, 0, 0> zzxx;
			Swizle_4<T, 2, 2, 0, 1> zzxy;
			Swizle_4<T, 2, 2, 0, 2> zzxz;
			Swizle_4<T, 2, 2, 1, 0> zzyx;
			Swizle_4<T, 2, 2, 1, 1> zzyy;
			Swizle_4<T, 2, 2, 1, 2> zzyz;
			Swizle_4<T, 2, 2, 2, 0> zzzx;
			Swizle_4<T, 2, 2, 2, 1> zzzy;
			Swizle_4<T, 2, 2, 2, 2> zzzz;
		};
	};

	constexpr Vector() : Vector(0) {}
	constexpr explicit Vector(T value) : x(value), y(value), z(value) {}
	constexpr Vector(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	constexpr Vector(const Vector<2, T>& vec2, T _z) : x(vec2.x), y(vec2.y), z(_z) {}
};

template<typename T>
struct Vector<4, T> : public math::TVector<4, T>
{
	union
	{
		struct
		{
			T x, y, z, w;
		};
		T data[4];

		// 2D swizzles
		union
		{
			Swizle_2<T, 0, 0> xx;
			Swizle_2<T, 0, 1> xy;
			Swizle_2<T, 0, 2> xz;
			Swizle_2<T, 0, 3> xw;
			Swizle_2<T, 1, 0> yx;
			Swizle_2<T, 1, 1> yy;
			Swizle_2<T, 1, 2> yz;
			Swizle_2<T, 1, 3> yw;
			Swizle_2<T, 2, 0> zx;
			Swizle_2<T, 2, 1> zy;
			Swizle_2<T, 2, 2> zz;
			Swizle_2<T, 2, 3> zw;
			Swizle_2<T, 3, 0> wx;
			Swizle_2<T, 3, 1> wy;
			Swizle_2<T, 3, 2> wz;
			Swizle_2<T, 3, 3> ww;
		};

		// 3D swizzles
		union
		{
			Swizle_3<T, 0, 0, 0> xxx;
			Swizle_3<T, 0, 0, 1> xxy;
			Swizle_3<T, 0, 0, 2> xxz;
			Swizle_3<T, 0, 0, 3> xxw;
			Swizle_3<T, 0, 1, 0> xyx;
			Swizle_3<T, 0, 1, 1> xyy;
			Swizle_3<T, 0, 1, 2> xyz;
			Swizle_3<T, 0, 1, 3> xyw;
			Swizle_3<T, 0, 2, 0> xzx;
			Swizle_3<T, 0, 2, 1> xzy;
			Swizle_3<T, 0, 2, 2> xzz;
			Swizle_3<T, 0, 2, 3> xzw;
			Swizle_3<T, 0, 3, 0> xwx;
			Swizle_3<T, 0, 3, 1> xwy;
			Swizle_3<T, 0, 3, 2> xwz;
			Swizle_3<T, 0, 3, 3> xww;
			Swizle_3<T, 1, 0, 0> yxx;
			Swizle_3<T, 1, 0, 1> yxy;
			Swizle_3<T, 1, 0, 2> yxz;
			Swizle_3<T, 1, 0, 3> yxw;
			Swizle_3<T, 1, 1, 0> yyx;
			Swizle_3<T, 1, 1, 1> yyy;
			Swizle_3<T, 1, 1, 2> yyz;
			Swizle_3<T, 1, 1, 3> yyw;
			Swizle_3<T, 1, 2, 0> yzx;
			Swizle_3<T, 1, 2, 1> yzy;
			Swizle_3<T, 1, 2, 2> yzz;
			Swizle_3<T, 1, 2, 3> yzw;
			Swizle_3<T, 1, 3, 0> ywx;
			Swizle_3<T, 1, 3, 1> ywy;
			Swizle_3<T, 1, 3, 2> ywz;
			Swizle_3<T, 1, 3, 3> yww;
			Swizle_3<T, 2, 0, 0> zxx;
			Swizle_3<T, 2, 0, 1> zxy;
			Swizle_3<T, 2, 0, 2> zxz;
			Swizle_3<T, 2, 0, 3> zxw;
			Swizle_3<T, 2, 1, 0> zyx;
			Swizle_3<T, 2, 1, 1> zyy;
			Swizle_3<T, 2, 1, 2> zyz;
			Swizle_3<T, 2, 1, 3> zyw;
			Swizle_3<T, 2, 2, 0> zzx;
			Swizle_3<T, 2, 2, 1> zzy;
			Swizle_3<T, 2, 2, 2> zzz;
			Swizle_3<T, 2, 2, 3> zzw;
			Swizle_3<T, 2, 3, 0> zwx;
			Swizle_3<T, 2, 3, 1> zwy;
			Swizle_3<T, 2, 3, 2> zwz;
			Swizle_3<T, 2, 3, 3> zww;
			Swizle_3<T, 3, 0, 0> wxx;
			Swizle_3<T, 3, 0, 1> wxy;
			Swizle_3<T, 3, 0, 2> wxz;
			Swizle_3<T, 3, 0, 3> wxw;
			Swizle_3<T, 3, 1, 0> wyx;
			Swizle_3<T, 3, 1, 1> wyy;
			Swizle_3<T, 3, 1, 2> wyz;
			Swizle_3<T, 3, 1, 3> wyw;
			Swizle_3<T, 3, 2, 0> wzx;
			Swizle_3<T, 3, 2, 1> wzy;
			Swizle_3<T, 3, 2, 2> wzz;
			Swizle_3<T, 3, 2, 3> wzw;
			Swizle_3<T, 3, 3, 0> wwx;
			Swizle_3<T, 3, 3, 1> wwy;
			Swizle_3<T, 3, 3, 2> wwz;
			Swizle_3<T, 3, 3, 3> www;
		};

		// 4D swizzles
		union
		{
			Swizle_4<T, 0, 0, 0, 0> xxxx;
			Swizle_4<T, 0, 0, 0, 1> xxxy;
			Swizle_4<T, 0, 0, 0, 2> xxxz;
			Swizle_4<T, 0, 0, 0, 3> xxxw;
			Swizle_4<T, 0, 0, 1, 0> xxyx;
			Swizle_4<T, 0, 0, 1, 1> xxyy;
			Swizle_4<T, 0, 0, 1, 2> xxyz;
			Swizle_4<T, 0, 0, 1, 3> xxyw;
			Swizle_4<T, 0, 0, 2, 0> xxzx;
			Swizle_4<T, 0, 0, 2, 1> xxzy;
			Swizle_4<T, 0, 0, 2, 2> xxzz;
			Swizle_4<T, 0, 0, 2, 3> xxzw;
			Swizle_4<T, 0, 0, 3, 0> xxwx;
			Swizle_4<T, 0, 0, 3, 1> xxwy;
			Swizle_4<T, 0, 0, 3, 2> xxwz;
			Swizle_4<T, 0, 0, 3, 3> xxww;
			Swizle_4<T, 0, 1, 0, 0> xyxx;
			Swizle_4<T, 0, 1, 0, 1> xyxy;
			Swizle_4<T, 0, 1, 0, 2> xyxz;
			Swizle_4<T, 0, 1, 0, 3> xyxw;
			Swizle_4<T, 0, 1, 1, 0> xyyx;
			Swizle_4<T, 0, 1, 1, 1> xyyy;
			Swizle_4<T, 0, 1, 1, 2> xyyz;
			Swizle_4<T, 0, 1, 1, 3> xyyw;
			Swizle_4<T, 0, 1, 2, 0> xyzx;
			Swizle_4<T, 0, 1, 2, 1> xyzy;
			Swizle_4<T, 0, 1, 2, 2> xyzz;
			Swizle_4<T, 0, 1, 2, 3> xyzw;
			Swizle_4<T, 0, 1, 3, 0> xywx;
			Swizle_4<T, 0, 1, 3, 1> xywy;
			Swizle_4<T, 0, 1, 3, 2> xywz;
			Swizle_4<T, 0, 1, 3, 3> xyww;
			Swizle_4<T, 0, 2, 0, 0> xzxx;
			Swizle_4<T, 0, 2, 0, 1> xzxy;
			Swizle_4<T, 0, 2, 0, 2> xzxz;
			Swizle_4<T, 0, 2, 0, 3> xzxw;
			Swizle_4<T, 0, 2, 1, 0> xzyx;
			Swizle_4<T, 0, 2, 1, 1> xzyy;
			Swizle_4<T, 0, 2, 1, 2> xzyz;
			Swizle_4<T, 0, 2, 1, 3> xzyw;
			Swizle_4<T, 0, 2, 2, 0> xzzx;
			Swizle_4<T, 0, 2, 2, 1> xzzy;
			Swizle_4<T, 0, 2, 2, 2> xzzz;
			Swizle_4<T, 0, 2, 2, 3> xzzw;
			Swizle_4<T, 0, 2, 3, 0> xzwx;
			Swizle_4<T, 0, 2, 3, 1> xzwy;
			Swizle_4<T, 0, 2, 3, 2> xzwz;
			Swizle_4<T, 0, 2, 3, 3> xzww;
			Swizle_4<T, 0, 3, 0, 0> xwxx;
			Swizle_4<T, 0, 3, 0, 1> xwxy;
			Swizle_4<T, 0, 3, 0, 2> xwxz;
			Swizle_4<T, 0, 3, 0, 3> xwxw;
			Swizle_4<T, 0, 3, 1, 0> xwyx;
			Swizle_4<T, 0, 3, 1, 1> xwyy;
			Swizle_4<T, 0, 3, 1, 2> xwyz;
			Swizle_4<T, 0, 3, 1, 3> xwyw;
			Swizle_4<T, 0, 3, 2, 0> xwzx;
			Swizle_4<T, 0, 3, 2, 1> xwzy;
			Swizle_4<T, 0, 3, 2, 2> xwzz;
			Swizle_4<T, 0, 3, 2, 3> xwzw;
			Swizle_4<T, 0, 3, 3, 0> xwwx;
			Swizle_4<T, 0, 3, 3, 1> xwwy;
			Swizle_4<T, 0, 3, 3, 2> xwwz;
			Swizle_4<T, 0, 3, 3, 3> xwww;
			Swizle_4<T, 1, 0, 0, 0> yxxx;
			Swizle_4<T, 1, 0, 0, 1> yxxy;
			Swizle_4<T, 1, 0, 0, 2> yxxz;
			Swizle_4<T, 1, 0, 0, 3> yxxw;
			Swizle_4<T, 1, 0, 1, 0> yxyx;
			Swizle_4<T, 1, 0, 1, 1> yxyy;
			Swizle_4<T, 1, 0, 1, 2> yxyz;
			Swizle_4<T, 1, 0, 1, 3> yxyw;
			Swizle_4<T, 1, 0, 2, 0> yxzx;
			Swizle_4<T, 1, 0, 2, 1> yxzy;
			Swizle_4<T, 1, 0, 2, 2> yxzz;
			Swizle_4<T, 1, 0, 2, 3> yxzw;
			Swizle_4<T, 1, 0, 3, 0> yxwx;
			Swizle_4<T, 1, 0, 3, 1> yxwy;
			Swizle_4<T, 1, 0, 3, 2> yxwz;
			Swizle_4<T, 1, 0, 3, 3> yxww;
			Swizle_4<T, 1, 1, 0, 0> yyxx;
			Swizle_4<T, 1, 1, 0, 1> yyxy;
			Swizle_4<T, 1, 1, 0, 2> yyxz;
			Swizle_4<T, 1, 1, 0, 3> yyxw;
			Swizle_4<T, 1, 1, 1, 0> yyyx;
			Swizle_4<T, 1, 1, 1, 1> yyyy;
			Swizle_4<T, 1, 1, 1, 2> yyyz;
			Swizle_4<T, 1, 1, 1, 3> yyyw;
			Swizle_4<T, 1, 1, 2, 0> yyzx;
			Swizle_4<T, 1, 1, 2, 1> yyzy;
			Swizle_4<T, 1, 1, 2, 2> yyzz;
			Swizle_4<T, 1, 1, 2, 3> yyzw;
			Swizle_4<T, 1, 1, 3, 0> yywx;
			Swizle_4<T, 1, 1, 3, 1> yywy;
			Swizle_4<T, 1, 1, 3, 2> yywz;
			Swizle_4<T, 1, 1, 3, 3> yyww;
			Swizle_4<T, 1, 2, 0, 0> yzxx;
			Swizle_4<T, 1, 2, 0, 1> yzxy;
			Swizle_4<T, 1, 2, 0, 2> yzxz;
			Swizle_4<T, 1, 2, 0, 3> yzxw;
			Swizle_4<T, 1, 2, 1, 0> yzyx;
			Swizle_4<T, 1, 2, 1, 1> yzyy;
			Swizle_4<T, 1, 2, 1, 2> yzyz;
			Swizle_4<T, 1, 2, 1, 3> yzyw;
			Swizle_4<T, 1, 2, 2, 0> yzzx;
			Swizle_4<T, 1, 2, 2, 1> yzzy;
			Swizle_4<T, 1, 2, 2, 2> yzzz;
			Swizle_4<T, 1, 2, 2, 3> yzzw;
			Swizle_4<T, 1, 2, 3, 0> yzwx;
			Swizle_4<T, 1, 2, 3, 1> yzwy;
			Swizle_4<T, 1, 2, 3, 2> yzwz;
			Swizle_4<T, 1, 2, 3, 3> yzww;
			Swizle_4<T, 1, 3, 0, 0> ywxx;
			Swizle_4<T, 1, 3, 0, 1> ywxy;
			Swizle_4<T, 1, 3, 0, 2> ywxz;
			Swizle_4<T, 1, 3, 0, 3> ywxw;
			Swizle_4<T, 1, 3, 1, 0> ywyx;
			Swizle_4<T, 1, 3, 1, 1> ywyy;
			Swizle_4<T, 1, 3, 1, 2> ywyz;
			Swizle_4<T, 1, 3, 1, 3> ywyw;
			Swizle_4<T, 1, 3, 2, 0> ywzx;
			Swizle_4<T, 1, 3, 2, 1> ywzy;
			Swizle_4<T, 1, 3, 2, 2> ywzz;
			Swizle_4<T, 1, 3, 2, 3> ywzw;
			Swizle_4<T, 1, 3, 3, 0> ywwx;
			Swizle_4<T, 1, 3, 3, 1> ywwy;
			Swizle_4<T, 1, 3, 3, 2> ywwz;
			Swizle_4<T, 1, 3, 3, 3> ywww;
			Swizle_4<T, 2, 0, 0, 0> zxxx;
			Swizle_4<T, 2, 0, 0, 1> zxxy;
			Swizle_4<T, 2, 0, 0, 2> zxxz;
			Swizle_4<T, 2, 0, 0, 3> zxxw;
			Swizle_4<T, 2, 0, 1, 0> zxyx;
			Swizle_4<T, 2, 0, 1, 1> zxyy;
			Swizle_4<T, 2, 0, 1, 2> zxyz;
			Swizle_4<T, 2, 0, 1, 3> zxyw;
			Swizle_4<T, 2, 0, 2, 0> zxzx;
			Swizle_4<T, 2, 0, 2, 1> zxzy;
			Swizle_4<T, 2, 0, 2, 2> zxzz;
			Swizle_4<T, 2, 0, 2, 3> zxzw;
			Swizle_4<T, 2, 0, 3, 0> zxwx;
			Swizle_4<T, 2, 0, 3, 1> zxwy;
			Swizle_4<T, 2, 0, 3, 2> zxwz;
			Swizle_4<T, 2, 0, 3, 3> zxww;
			Swizle_4<T, 2, 1, 0, 0> zyxx;
			Swizle_4<T, 2, 1, 0, 1> zyxy;
			Swizle_4<T, 2, 1, 0, 2> zyxz;
			Swizle_4<T, 2, 1, 0, 3> zyxw;
			Swizle_4<T, 2, 1, 1, 0> zyyx;
			Swizle_4<T, 2, 1, 1, 1> zyyy;
			Swizle_4<T, 2, 1, 1, 2> zyyz;
			Swizle_4<T, 2, 1, 1, 3> zyyw;
			Swizle_4<T, 2, 1, 2, 0> zyzx;
			Swizle_4<T, 2, 1, 2, 1> zyzy;
			Swizle_4<T, 2, 1, 2, 2> zyzz;
			Swizle_4<T, 2, 1, 2, 3> zyzw;
			Swizle_4<T, 2, 1, 3, 0> zywx;
			Swizle_4<T, 2, 1, 3, 1> zywy;
			Swizle_4<T, 2, 1, 3, 2> zywz;
			Swizle_4<T, 2, 1, 3, 3> zyww;
			Swizle_4<T, 2, 2, 0, 0> zzxx;
			Swizle_4<T, 2, 2, 0, 1> zzxy;
			Swizle_4<T, 2, 2, 0, 2> zzxz;
			Swizle_4<T, 2, 2, 0, 3> zzxw;
			Swizle_4<T, 2, 2, 1, 0> zzyx;
			Swizle_4<T, 2, 2, 1, 1> zzyy;
			Swizle_4<T, 2, 2, 1, 2> zzyz;
			Swizle_4<T, 2, 2, 1, 3> zzyw;
			Swizle_4<T, 2, 2, 2, 0> zzzx;
			Swizle_4<T, 2, 2, 2, 1> zzzy;
			Swizle_4<T, 2, 2, 2, 2> zzzz;
			Swizle_4<T, 2, 2, 2, 3> zzzw;
			Swizle_4<T, 2, 2, 3, 0> zzwx;
			Swizle_4<T, 2, 2, 3, 1> zzwy;
			Swizle_4<T, 2, 2, 3, 2> zzwz;
			Swizle_4<T, 2, 2, 3, 3> zzww;
			Swizle_4<T, 2, 3, 0, 0> zwxx;
			Swizle_4<T, 2, 3, 0, 1> zwxy;
			Swizle_4<T, 2, 3, 0, 2> zwxz;
			Swizle_4<T, 2, 3, 0, 3> zwxw;
			Swizle_4<T, 2, 3, 1, 0> zwyx;
			Swizle_4<T, 2, 3, 1, 1> zwyy;
			Swizle_4<T, 2, 3, 1, 2> zwyz;
			Swizle_4<T, 2, 3, 1, 3> zwyw;
			Swizle_4<T, 2, 3, 2, 0> zwzx;
			Swizle_4<T, 2, 3, 2, 1> zwzy;
			Swizle_4<T, 2, 3, 2, 2> zwzz;
			Swizle_4<T, 2, 3, 2, 3> zwzw;
			Swizle_4<T, 2, 3, 3, 0> zwwx;
			Swizle_4<T, 2, 3, 3, 1> zwwy;
			Swizle_4<T, 2, 3, 3, 2> zwwz;
			Swizle_4<T, 2, 3, 3, 3> zwww;
			Swizle_4<T, 3, 0, 0, 0> wxxx;
			Swizle_4<T, 3, 0, 0, 1> wxxy;
			Swizle_4<T, 3, 0, 0, 2> wxxz;
			Swizle_4<T, 3, 0, 0, 3> wxxw;
			Swizle_4<T, 3, 0, 1, 0> wxyx;
			Swizle_4<T, 3, 0, 1, 1> wxyy;
			Swizle_4<T, 3, 0, 1, 2> wxyz;
			Swizle_4<T, 3, 0, 1, 3> wxyw;
			Swizle_4<T, 3, 0, 2, 0> wxzx;
			Swizle_4<T, 3, 0, 2, 1> wxzy;
			Swizle_4<T, 3, 0, 2, 2> wxzz;
			Swizle_4<T, 3, 0, 2, 3> wxzw;
			Swizle_4<T, 3, 0, 3, 0> wxwx;
			Swizle_4<T, 3, 0, 3, 1> wxwy;
			Swizle_4<T, 3, 0, 3, 2> wxwz;
			Swizle_4<T, 3, 0, 3, 3> wxww;
			Swizle_4<T, 3, 1, 0, 0> wyxx;
			Swizle_4<T, 3, 1, 0, 1> wyxy;
			Swizle_4<T, 3, 1, 0, 2> wyxz;
			Swizle_4<T, 3, 1, 0, 3> wyxw;
			Swizle_4<T, 3, 1, 1, 0> wyyx;
			Swizle_4<T, 3, 1, 1, 1> wyyy;
			Swizle_4<T, 3, 1, 1, 2> wyyz;
			Swizle_4<T, 3, 1, 1, 3> wyyw;
			Swizle_4<T, 3, 1, 2, 0> wyzx;
			Swizle_4<T, 3, 1, 2, 1> wyzy;
			Swizle_4<T, 3, 1, 2, 2> wyzz;
			Swizle_4<T, 3, 1, 2, 3> wyzw;
			Swizle_4<T, 3, 1, 3, 0> wywx;
			Swizle_4<T, 3, 1, 3, 1> wywy;
			Swizle_4<T, 3, 1, 3, 2> wywz;
			Swizle_4<T, 3, 1, 3, 3> wyww;
			Swizle_4<T, 3, 2, 0, 0> wzxx;
			Swizle_4<T, 3, 2, 0, 1> wzxy;
			Swizle_4<T, 3, 2, 0, 2> wzxz;
			Swizle_4<T, 3, 2, 0, 3> wzxw;
			Swizle_4<T, 3, 2, 1, 0> wzyx;
			Swizle_4<T, 3, 2, 1, 1> wzyy;
			Swizle_4<T, 3, 2, 1, 2> wzyz;
			Swizle_4<T, 3, 2, 1, 3> wzyw;
			Swizle_4<T, 3, 2, 2, 0> wzzx;
			Swizle_4<T, 3, 2, 2, 1> wzzy;
			Swizle_4<T, 3, 2, 2, 2> wzzz;
			Swizle_4<T, 3, 2, 2, 3> wzzw;
			Swizle_4<T, 3, 2, 3, 0> wzwx;
			Swizle_4<T, 3, 2, 3, 1> wzwy;
			Swizle_4<T, 3, 2, 3, 2> wzwz;
			Swizle_4<T, 3, 2, 3, 3> wzww;
			Swizle_4<T, 3, 3, 0, 0> wwxx;
			Swizle_4<T, 3, 3, 0, 1> wwxy;
			Swizle_4<T, 3, 3, 0, 2> wwxz;
			Swizle_4<T, 3, 3, 0, 3> wwxw;
			Swizle_4<T, 3, 3, 1, 0> wwyx;
			Swizle_4<T, 3, 3, 1, 1> wwyy;
			Swizle_4<T, 3, 3, 1, 2> wwyz;
			Swizle_4<T, 3, 3, 1, 3> wwyw;
			Swizle_4<T, 3, 3, 2, 0> wwzx;
			Swizle_4<T, 3, 3, 2, 1> wwzy;
			Swizle_4<T, 3, 3, 2, 2> wwzz;
			Swizle_4<T, 3, 3, 2, 3> wwzw;
			Swizle_4<T, 3, 3, 3, 0> wwwx;
			Swizle_4<T, 3, 3, 3, 1> wwwy;
			Swizle_4<T, 3, 3, 3, 2> wwwz;
			Swizle_4<T, 3, 3, 3, 3> wwww;
		};
	};

	constexpr Vector() : Vector(0) {}
	constexpr explicit Vector(const T value) : x(value), y(value), z(value), w(value) {}
	constexpr Vector(const T _x, const T _y, const T _z, const T _w) : x(_x), y(_y), z(_z), w(_w) {}
	constexpr Vector(const Vector<3, T>& vec3, const T _w) : x(vec3.x), y(vec3.y), z(vec3.z), w(_w) {}
	constexpr Vector(const Vector<2, T>& vec2, const T _z, const T _w) : x(vec2.x), y(vec2.y), z(_z), w(_w) {}
};

template<size_t size, typename T>
constexpr bool operator==(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		if (lhs[i] != rhs[i])return false;
	}
	return true;
}

template<size_t size, typename T>
constexpr bool operator!=(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		if (lhs[i] != rhs[i])return true;
	}
	return false;
}

template<size_t size, typename T>
constexpr Vector<size, T>& operator+=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] += rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
constexpr Vector<size, T>& operator-=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] -= rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
constexpr Vector<size, T>& operator*=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] *= rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
constexpr Vector<size, T>& operator*=(Vector<size, T>& lhs, const T rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] *= rhs;
	}
	return lhs;
}

template<size_t size, typename T>
constexpr Vector<size, T>& operator/=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] /= rhs[i];
	}
	return lhs;
}

template<size_t size, typename T>
constexpr Vector<size, T>& operator/=(Vector<size, T>& lhs, const T rhs) {
	for (size_t i = 0; i < size; i++)
	{
		lhs[i] /= rhs;
	}
	return lhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator+(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs += rhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator-(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs -= rhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator*(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs *= rhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator*(Vector<size, T> lhs, const T rhs) {
	return lhs *= rhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator*(const T lhs, Vector<size, T> rhs) {
	return rhs *= lhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator/(Vector<size, T> lhs, const Vector<size, T>& rhs) {
	return lhs /= rhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator/(Vector<size, T> lhs, const T rhs) {
	return lhs /= rhs;
}

template<size_t size, typename T>
constexpr Vector<size, T> operator-(Vector<size, T> rhs) {
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

using Vector2i = Vector<2, int>;
using Vector2  = Vector<2, float>;
using Vector2f = Vector<2, float>;
using Vector2d = Vector<2, double>;

using Vector3i = Vector<3, int>;
using Vector3  = Vector<3, float>;
using Vector3f = Vector<3, float>;
using Vector3d = Vector<3, double>;

using Vector4i = Vector<4, int>;
using Vector4  = Vector<4, float>;
using Vector4f = Vector<4, float>;
using Vector4d = Vector<4, double>;

// template <typename T> const Vector<T> Vector<T>::right(1, 0, 0);
// template <typename T> const Vector<T> Vector<T>::up(0, 1, 0);
// template <typename T> const Vector<T> Vector<T>::forward(0, 0, 1);
