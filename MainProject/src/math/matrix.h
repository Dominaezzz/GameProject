#pragma once

#include "vector.h"
#include "quaternion.h"

template<size_t rows, size_t columns, typename T>
struct Matrix;

namespace math
{
	template<typename T, size_t rows, size_t columns>
	struct TMatrix {
		Vector<rows, T>& operator[](const size_t column)
		{
			return static_cast<Matrix<rows, columns, T>*>(this)->columns[column];
		}
		const Vector<rows, T>& operator[](const size_t column) const
		{
			return static_cast<const Matrix<rows, columns, T>*>(this)->columns[column];
		}

		T& operator()(const size_t row, const size_t column) {
			return (*this)[column][row];
		}
		const T& operator()(const size_t row, const size_t column) const {
			return (*this)[column][row];
		}

		static Matrix<rows, columns, T> scale(const Vector<rows, T>& vector) {
			Matrix<rows, columns, T> result;
			for (size_t row = 0; row < rows; row++)
			{
				for (size_t column = 0; column < columns; column++)
				{
					result(row, column) = (row == column) ? vector[row] : 0;
				}
			}
			return scale;
		}
		static Matrix<rows, columns, T> scale(const T scale) {
			return Matrix<rows, columns, T>(scale);
		}

		Matrix<columns, rows, T> transposed() {
			Matrix<columns, rows, T> result;
			for (size_t row = 0; row < rows; row++)
			{
				for (size_t column = 0; column < columns; column++)
				{
					result(row, column) = (*this)(column, row);
				}
			}
			return result;
		}
		Matrix<rows, columns, T>& transpose() {
			static_assert(rows == columns, "Can only transpose() square matrices.");

			for (size_t row = 0; row < rows; row++)
			{
				for (size_t column = row + 1; column < columns; column++)
				{
					std::swap((*this)(row, column), (*this)(column, row));
				}
			}
			return *static_cast<Matrix<rows, columns, T>*>(this);
		}

		friend Vector<rows, T> operator*(const Matrix<rows, columns, T>& lhs, const Vector<columns, T>& vector) {
			Vector<rows, T> result(0);
			for (size_t row = 0; row < 4; row++)
			{
				for (size_t column = 0; column < 4; column++)
				{
					result[row] += lhs(row, column) * vector[column];
				}
			}
			return result;
		}
		Vector<rows - 1, T> transform(const Vector<columns - 1, T>& vector, const T w = 1) const {
			Vector<rows - 1, T> result(0);
			for (size_t row = 0; row < rows - 1; row++)
			{
				for (size_t column = 0; column < columns - 1; column++)
				{
					result[row] += (*this)(row, column) * vector[column];
				}
				result[row] += (*this)(row, columns - 1) * w;
			}
			return result;
		}
	};
}

template<size_t rows, size_t ncolumns, typename T>
struct Matrix : public math::TMatrix<T, rows, ncolumns> {
	union
	{
		T data[rows * ncolumns];
		Vector<rows, T> columns[ncolumns];
	};
	
	constexpr Matrix() = default;
	constexpr explicit Matrix(T diagonal) {
		for (size_t row = 0; row < rows; row++)
		{
			for (size_t column = 0; column < ncolumns; column++)
			{
				(*this)(row, column) = (row == column) ? diagonal : 0;
			}
		}
	}

	constexpr explicit Matrix(const T _data[rows * ncolumns]) {
		for (size_t i = 0; i < rows * ncolumns; i++)
		{
			data[i] = _data[i];
		}
	}
};

template<typename T>
struct Matrix<2, 2, T> : public math::TMatrix<T, 2, 2> {
	union
	{
		T data[2 * 2];
		Vector<2, T> columns[2];
		struct
		{
			T m00, m10;
			T m01, m11;
		};
	};

	constexpr Matrix() : Matrix(1) {}
	constexpr explicit Matrix(T diagonal) : m00(diagonal), m01(0), m10(0), m11(diagonal) {}
	constexpr Matrix(T _00, T _01, T _10, T _11) : m00(_00), m01(_01), m10(_10), m11(_11) {}

	constexpr Matrix<2, 2, T> inverted() const {
		T determinant = (m00 * m11) - (m01 * m10);
		if (determinant != 0)
		{
			return Matrix<2, 2, T>(
				m11 / determinant, -m01 / determinant,
				-m10 / determinant, m00 / determinant
				);
		}
		throw std::logic_error("Cannot invert singular matrix");
	}
	constexpr Matrix<2, 2, T>& invert() {
		return (*this) = inverted();
	}

	constexpr static Matrix<2, 2, T> rotateZ(const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix<2, 2, T>(
			c, -s,
			s, c
			);
	}
	constexpr static Matrix<2, 2, T> scale(const Vector<2, T>& vector) {
		return Matrix<2, 2, T>(
			vector.x, 0,
			0, vector.y
			);
	}
};

template<typename T>
struct Matrix<3, 3, T> : public math::TMatrix<T, 3, 3> {
	union {
		T data[3 * 3];
		Vector<3, T> columns[3];
		struct
		{
			T m00, m10, m20;
			T m01, m11, m21;
			T m02, m12, m22;
		};
	};

	constexpr Matrix() : Matrix(1) {}
	constexpr explicit Matrix(T diagonal) :
		m00(diagonal), m01(0), m02(0),
		m10(0), m11(diagonal), m12(0),
		m20(0), m21(0), m22(diagonal) {}
	constexpr Matrix(
		T _00, T _01, T _02,
		T _10, T _11, T _12,
		T _20, T _21, T _22
	) : m00(_00), m01(_01), m02(_02),
		m10(_10), m11(_11), m12(_12),
		m20(_20), m21(_21), m22(_22) {}

	constexpr T determinant() const {
		T d = 0;

		//either this
		d += (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1));
		d += (*this)(0, 1) * ((*this)(1, 2) * (*this)(2, 0) - (*this)(1, 0) * (*this)(2, 2));
		d += (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));

		//or this

		//  for (size_t i = 0; i < 3; i++) {
		//  	size_t left = (i + 1) % 3;
		//  	size_t right = (i + 2) % 3;
		//  	d += (*this)(0, i) * ((*this)(1, left) * (*this)(2, right) - (*this)(1, right) * (*this)(2, left));
		//  }

		return d;
	}
	constexpr Matrix inverted() const {
		T d = determinant();
		if (d != 0)
		{
			Matrix result(
				(*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1),
				(*this)(0, 2) * (*this)(2, 1) - (*this)(0, 1) * (*this)(2, 2),
				(*this)(0, 1) * (*this)(1, 2) - (*this)(0, 2) * (*this)(1, 1),

				(*this)(1, 2) * (*this)(2, 0) - (*this)(1, 0) * (*this)(2, 2),
				(*this)(0, 0) * (*this)(2, 2) - (*this)(0, 2) * (*this)(2, 0),
				(*this)(0, 2) * (*this)(1, 0) - (*this)(0, 0) * (*this)(1, 2),

				(*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0),
				(*this)(0, 1) * (*this)(2, 0) - (*this)(0, 0) * (*this)(2, 1),
				(*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0)
			);
			return result /= d;
		}
		throw std::logic_error("Cannot invert singular matrix");
	}
	constexpr Matrix& invert() {
		return *this = inverted();
	}

	constexpr static Matrix rotate(const Vector<3, T>& vector, const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix(
			(vector.x * vector.x)*(1 - c) + c, (vector.x * vector.y)*(1 - c) - (vector.z * s), (vector.x * vector.y)*(1 - c) + (vector.y * s),

			(vector.x * vector.y)*(1 - c) + (vector.z * s), (vector.y * vector.y)*(1 - c) + c, (vector.y * vector.z)*(1 - c) - (vector.x * s),

			(vector.x * vector.z)*(1 - c) - (vector.y * s), (vector.y * vector.z)*(1 - c) + (vector.x * s), (vector.z * vector.z)*(1 - c) + c
		);
	}
	constexpr static Matrix rotateX(const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix(
			1, 0, 0,
			0, c, -s,
			0, s, c
		);
	}
	constexpr static Matrix rotateY(const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix(
			c, 0, s,
			0, 1, 0,
			-s, 0, c
		);
	}
	constexpr static Matrix rotateZ(const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix(
			c, -s, 0,
			s, c, 0,
			0, 0, 1
		);
	}
	constexpr static Matrix scale(const Vector<3, T>& vector) {
		return Matrix(
			vector.x, 0, 0,
			0, vector.y, 0,
			0, 0, vector.z
		);
	}
};

template<typename T>
struct Matrix<4, 4, T> : public math::TMatrix<T, 4, 4> {
	union
	{
		T data[4 * 4];
		Vector<4, T> columns[4];
		struct
		{
			T m00, m10, m20, m30;
			T m01, m11, m21, m31;
			T m02, m12, m22, m32;
			T m03, m13, m23, m33;
		};
	};

	constexpr Matrix() : Matrix(1) {}
	constexpr explicit Matrix(T diagonal) :
		m00(diagonal), m01(0), m02(0), m03(0),
		m10(0), m11(diagonal), m12(0), m13(0),
		m20(0), m21(0), m22(diagonal), m23(0),
		m30(0), m31(0), m32(0), m33(diagonal) {}
	constexpr Matrix(
		T _00, T _01, T _02, T _03,
		T _10, T _11, T _12, T _13,
		T _20, T _21, T _22, T _23,
		T _30, T _31, T _32, T _33
	) : m00(_00), m01(_01), m02(_02), m03(_03),
		m10(_10), m11(_11), m12(_12), m13(_13),
		m20(_20), m21(_21), m22(_22), m23(_23),
		m30(_30), m31(_31), m32(_32), m33(_33) {}

private:
	constexpr static T determinant3x3(T t00, T t01, T t02, T t10, T t11, T t12, T t20, T t21, T t22)
	{
		return t00 * (t11 * t22 - t12 * t21) + t01 * (t12 * t20 - t10 * t22) + t02 * (t10 * t21 - t11 * t20);
	}
public:
	constexpr T determinant() const {
		T f = (*this)(0, 0) * (
			+(*this)(1, 1) * (*this)(2, 2) * (*this)(3, 3)
			+ (*this)(1, 2) * (*this)(2, 3) * (*this)(3, 1)
			+ (*this)(1, 3) * (*this)(2, 1) * (*this)(3, 2)
			- (*this)(1, 3) * (*this)(2, 2) * (*this)(3, 1)
			- (*this)(1, 1) * (*this)(2, 3) * (*this)(3, 2)
			- (*this)(1, 2) * (*this)(2, 1) * (*this)(3, 3)
			);

		f -= (*this)(0, 1) * (
			+(*this)(1, 0) * (*this)(2, 2) * (*this)(3, 3)
			+ (*this)(1, 2) * (*this)(2, 3) * (*this)(3, 0)
			+ (*this)(1, 3) * (*this)(2, 0) * (*this)(3, 2)
			- (*this)(1, 3) * (*this)(2, 2) * (*this)(3, 0)
			- (*this)(1, 0) * (*this)(2, 3) * (*this)(3, 2)
			- (*this)(1, 2) * (*this)(2, 0) * (*this)(3, 3)
			);

		f += (*this)(0, 2) * (
			+(*this)(1, 0) * (*this)(2, 1) * (*this)(3, 3)
			+ (*this)(1, 1) * (*this)(2, 3) * (*this)(3, 0)
			+ (*this)(1, 3) * (*this)(2, 0) * (*this)(3, 1)
			- (*this)(1, 3) * (*this)(2, 1) * (*this)(3, 0)
			- (*this)(1, 0) * (*this)(2, 3) * (*this)(3, 1)
			- (*this)(1, 1) * (*this)(2, 0) * (*this)(3, 3)
			);

		f -= (*this)(0, 3) * (
			+(*this)(1, 0) * (*this)(2, 1) * (*this)(3, 2)
			+ (*this)(1, 1) * (*this)(2, 2) * (*this)(3, 0)
			+ (*this)(1, 2) * (*this)(2, 0) * (*this)(3, 1)
			- (*this)(1, 2) * (*this)(2, 1) * (*this)(3, 0)
			- (*this)(1, 0) * (*this)(2, 2) * (*this)(3, 1)
			- (*this)(1, 1) * (*this)(2, 0) * (*this)(3, 2)
			);

		return f;
	}
	constexpr Matrix<4, 4, T> inverted() const {
		T d = determinant();
		if (d != 0)
		{
			Matrix<4, 4, T> result(
				determinant3x3((*this)(1, 1), (*this)(1, 2), (*this)(1, 3), (*this)(2, 1), (*this)(2, 2), (*this)(2, 3), (*this)(3, 1), (*this)(3, 2), (*this)(3, 3)),
				-determinant3x3((*this)(0, 1), (*this)(0, 2), (*this)(0, 3), (*this)(2, 1), (*this)(2, 2), (*this)(2, 3), (*this)(3, 1), (*this)(3, 2), (*this)(3, 3)),
				determinant3x3((*this)(0, 1), (*this)(0, 2), (*this)(0, 3), (*this)(1, 1), (*this)(1, 2), (*this)(1, 3), (*this)(3, 1), (*this)(3, 2), (*this)(3, 3)),
				-determinant3x3((*this)(0, 1), (*this)(0, 2), (*this)(0, 3), (*this)(1, 1), (*this)(1, 2), (*this)(1, 3), (*this)(2, 1), (*this)(2, 2), (*this)(2, 3)),

				-determinant3x3((*this)(1, 0), (*this)(1, 2), (*this)(1, 3), (*this)(2, 0), (*this)(2, 2), (*this)(2, 3), (*this)(3, 0), (*this)(3, 2), (*this)(3, 3)),
				determinant3x3((*this)(0, 0), (*this)(0, 2), (*this)(0, 3), (*this)(2, 0), (*this)(2, 2), (*this)(2, 3), (*this)(3, 0), (*this)(3, 2), (*this)(3, 3)),
				-determinant3x3((*this)(0, 0), (*this)(0, 2), (*this)(0, 3), (*this)(1, 0), (*this)(1, 2), (*this)(1, 3), (*this)(3, 0), (*this)(3, 2), (*this)(3, 3)),
				determinant3x3((*this)(0, 0), (*this)(0, 2), (*this)(0, 3), (*this)(1, 0), (*this)(1, 2), (*this)(1, 3), (*this)(2, 0), (*this)(2, 2), (*this)(2, 3)),

				determinant3x3((*this)(1, 0), (*this)(1, 1), (*this)(1, 3), (*this)(2, 0), (*this)(2, 1), (*this)(2, 3), (*this)(3, 0), (*this)(3, 1), (*this)(3, 3)),
				-determinant3x3((*this)(0, 0), (*this)(0, 1), (*this)(0, 3), (*this)(2, 0), (*this)(2, 1), (*this)(2, 3), (*this)(3, 0), (*this)(3, 1), (*this)(3, 3)),
				determinant3x3((*this)(0, 0), (*this)(0, 1), (*this)(0, 3), (*this)(1, 0), (*this)(1, 1), (*this)(1, 3), (*this)(3, 0), (*this)(3, 1), (*this)(3, 3)),
				-determinant3x3((*this)(0, 0), (*this)(0, 1), (*this)(0, 3), (*this)(1, 0), (*this)(1, 1), (*this)(1, 3), (*this)(2, 0), (*this)(2, 1), (*this)(2, 3)),

				-determinant3x3((*this)(1, 0), (*this)(1, 1), (*this)(1, 2), (*this)(2, 0), (*this)(2, 1), (*this)(2, 2), (*this)(3, 0), (*this)(3, 1), (*this)(3, 2)),
				determinant3x3((*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(2, 0), (*this)(2, 1), (*this)(2, 2), (*this)(3, 0), (*this)(3, 1), (*this)(3, 2)),
				-determinant3x3((*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(1, 0), (*this)(1, 1), (*this)(1, 2), (*this)(3, 0), (*this)(3, 1), (*this)(3, 2)),
				determinant3x3((*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(1, 0), (*this)(1, 1), (*this)(1, 2), (*this)(2, 0), (*this)(2, 1), (*this)(2, 2))
			);

			return result /= d;
		}
		throw std::logic_error("Cannot invert singular matrix");
	}
	constexpr Matrix<4, 4, T>& invert() {
		return (*this) = inverted();
	}

	constexpr static Matrix<4, 4, T> translate(const Vector<3, T>& vector) {
		return Matrix<4, 4, T>(
			1, 0, 0, vector.x,
			0, 1, 0, vector.y,
			0, 0, 1, vector.z,
			0, 0, 0, 1
		);
	}
	constexpr static Matrix<4, 4, T> rotate(const Vector<3, T>& vector, const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix<4, 4, T>(
			(vector.x * vector.x)*(1 - c) + c, (vector.x * vector.y)*(1 - c) - (vector.z * s), (vector.x * vector.z)*(1 - c) + (vector.y * s), 0,

			(vector.x * vector.y)*(1 - c) + (vector.z * s), (vector.y * vector.y)*(1 - c) + c, (vector.y * vector.z)*(1 - c) - (vector.x * s), 0,

			(vector.x * vector.z)*(1 - c) - (vector.y * s), (vector.y * vector.z)*(1 - c) + (vector.x * s), (vector.z * vector.z)*(1 - c) + c, 0,

			0, 0, 0, 1
		);
	}
	constexpr static Matrix<4, 4, T> rotateX(const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix<4, 4, T>(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
			);
	}
	constexpr static Matrix<4, 4, T> rotateY(const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix<4, 4, T>(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1
			);
	}
	constexpr static Matrix<4, 4, T> rotateZ(const T angle) {
		T s = sin(angle);
		T c = cos(angle);

		return Matrix<4, 4, T>(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
	}
	constexpr static Matrix<4, 4, T> createFromQuaternion(const TQuaternion<T>& quaternion)
	{
		Vector<3, T> axis;
		T angle;
		quaternion.toAxisAngle(axis, angle);
		return rotate(axis, angle);
	}
	constexpr static Matrix<4, 4, T> scale(const Vector<3, T>& vector) {
		return Matrix<4, 4, T>(
			vector.x, 0, 0, 0,
			0, vector.y, 0, 0,
			0, 0, vector.z, 0,
			0, 0, 0, 1
			);
	}
	constexpr static Matrix<4, 4, T> perspective(const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) {
		T x = (2 * zNear) / (right - left);
		T y = (2 * zNear) / (top - bottom);
		T a = (right + left) / (right - left);
		T b = (top + bottom) / (top - bottom);
		T c = -(zFar + zNear) / (zFar - zNear);
		T d = -(2 * zFar * zNear) / (zFar - zNear);

		return Matrix<4, 4, T>(
			x, 0, a, 0,
			0, y, b, 0,
			0, 0, c, d,
			0, 0, -1, 0
		);
	}
	constexpr static Matrix<4, 4, T> perspective(const T fovy, const T aspect, const T zNear, const T zFar) {
		T yMax = zNear * tan(fovy / 2);
		T yMin = -yMax;
		T xMin = yMin * aspect;
		T xMax = yMax * aspect;

		return perspective(xMin, xMax, yMin, yMax, zNear, zFar);
	}
	constexpr static Matrix<4, 4, T> orthographic(const T left, const T right, const T bottom, const T top, const T zNear, const T zFar) {
		T invX = 1 / (right - left);
		T invY = 1 / (top - bottom);
		T invZ = 1 / (zFar - zNear);

		return Matrix<4, 4, T>(
			2 * invX, 0, 0, -(right + left) * invX,
			0, 2 * invY, 0, -(top + bottom) * invY,
			0, 0, -2 * invZ, -(zFar + zNear) * invZ,
			0, 0, 0, 1
			);
	}
	constexpr static Matrix<4, 4, T> orthographic(const T width, const T height, const T zNear, const T zFar) {
		return orthographic(-width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
	}
	constexpr static Matrix<4, 4, T> lookAt(const Vector<3, T>& eye, const Vector<3, T>& target, const Vector<3, T>& up) {
		Vector<3, T> cameraZ = (eye - target).normalized();
		Vector<3, T> cameraX = Vector<3, T>::cross(up, cameraZ).normalized();
		Vector<3, T> cameraY = Vector<3, T>::cross(cameraZ, cameraX).normalized();

		return Matrix<4, 4, T>(
			cameraX.x, cameraX.y, cameraX.z, -Vector<3, T>::dot(eye, cameraX),
			cameraY.x, cameraY.y, cameraY.z, -Vector<3, T>::dot(eye, cameraY),
			cameraZ.x, cameraZ.y, cameraZ.z, -Vector<3, T>::dot(eye, cameraZ),
			0, 0, 0, 1
			);
	}
	Vector<3, T> project(const Vector<3, T>& vector) {
		Vector<3, T> result(0);
		for (size_t row = 0; row < 3; row++)
		{
			for (size_t column = 0; column < 3; column++)
			{
				result[row] += (*this)(row, column) * vector[column];
			}
			result[row] += (*this)(row, 4);
		}

		T w = (*this)(3, 3);
		for (size_t column = 0; column < 3; column++)
		{
			w += (*this)(3, column) * vector[column];
		}

		return result /= w;
	}
	Vector<3, T> extractTranslation() const
	{
		return Vector<3, T>((*this)(0, 3), (*this)(1, 3), (*this)(2, 3));
	}
	Vector<3, T> extractScale() const
	{
		Vector<3, T> result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result[i] += (*this)(i, j) * (*this)(i, j);
			}
			result[i] = sqrt(result[i]);
		}
		return result;
	}
	TQuaternion<T> extractRotation(bool row_normalise = true) const
	{
		Vector<3, T> column0((*this)(0, 0), (*this)(1, 0), (*this)(2, 0));// Row0.Xyz;
		Vector<3, T> column1((*this)(0, 1), (*this)(1, 1), (*this)(2, 1));// Row1.Xyz;
		Vector<3, T> column2((*this)(0, 2), (*this)(1, 2), (*this)(2, 2));// Row2.Xyz;

		if (row_normalise)
		{
			column0 = column0.normalized();
			column1 = column1.normalized();
			column2 = column2.normalized();
		}

		// code below adapted from Blender

		Quaternion q;
		double trace = 0.25 * (column0[0] + column1[1] + column2[2] + 1.0);

		if (trace > 0)
		{
			double sq = sqrt(trace);

			q.w = T(sq);
			sq = 1.0 / (4.0 * sq);
			q.x = T((column1[2] - column2[1]) * sq);
			q.y = T((column2[0] - column0[2]) * sq);
			q.z = T((column0[1] - column1[0]) * sq);
		}
		else if (column0[0] > column1[1] && column0[0] > column2[2])
		{
			double sq = 2.0 * sqrt(1.0 + column0[0] - column1[1] - column2[2]);

			q.x = T(0.25 * sq);
			sq = 1.0 / sq;
			q.w = T((column2[1] - column1[2]) * sq);
			q.y = T((column1[0] + column0[1]) * sq);
			q.z = T((column2[0] + column0[2]) * sq);
		}
		else if (column1[1] > column2[2])
		{
			double sq = 2.0 * sqrt(1.0 + column1[1] - column0[0] - column2[2]);

			q.y = T(0.25 * sq);
			sq = 1.0 / sq;
			q.w = T((column2[0] - column0[2]) * sq);
			q.x = T((column1[0] + column0[1]) * sq);
			q.z = T((column2[1] + column1[2]) * sq);
		}
		else
		{
			double sq = 2.0 * sqrt(1.0 + column2[2] - column0[0] - column1[1]);

			q.z = T(0.25 * sq);
			sq = 1.0 / sq;
			q.w = T((column1[0] - column0[1]) * sq);
			q.x = T((column2[0] + column0[2]) * sq);
			q.y = T((column2[1] + column1[2]) * sq);
		}

		q.normalize();
		return q;
	}
};

template<typename T, size_t rows, size_t columns>
constexpr bool operator==(const Matrix<rows, columns, T>& lhs, const Matrix<rows, columns, T>& rhs) {
	for (size_t i = 0; i < columns; i++)
	{
		if (lhs[i] != rhs[i])return false;
	}
	return true;
}

template<typename T, size_t rows, size_t columns>
constexpr bool operator!=(const Matrix<rows, columns, T>& lhs, const Matrix<rows, columns, T>& rhs) {
	for (size_t i = 0; i < columns; i++)
	{
		if (lhs[i] != rhs[i])return true;
	}
	return false;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T>& operator+=(Matrix<rows, columns, T>& lhs, const Matrix<rows, columns, T>& rhs) {
	for (int i = 0; i < columns; ++i) {
		lhs[i] += rhs[i];
	}
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T>& operator-=(Matrix<rows, columns, T>& lhs, const Matrix<rows, columns, T>& rhs) {
	for (int i = 0; i < columns; ++i) {
		lhs[i] -= rhs;
	}
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T>& operator*=(Matrix<rows, columns, T>& lhs, const T rhs) {
	for (size_t i = 0; i < columns; i++)
	{
		lhs[i] *= rhs;
	}
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T>& operator/=(Matrix<rows, columns, T>& lhs, const T rhs) {
	for (size_t i = 0; i < columns; i++)
	{
		lhs[i] /= rhs;
	}
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T> operator+(Matrix<rows, columns, T> lhs, const Matrix<rows, columns, T>& rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T> operator-(Matrix<rows, columns, T> lhs, const Matrix<rows, columns, T>& rhs) {
	lhs -= rhs;
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T> operator*(Matrix<rows, columns, T> lhs, const T rhs) {
	lhs *= rhs;
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T> operator/(Matrix<rows, columns, T> lhs, const T rhs) {
	lhs /= rhs;
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T> operator-(Matrix<rows, columns, T> lhs) {
	for (int i = 0; i < columns; ++i) {
		lhs[i] = -lhs[i];
	}
	return lhs;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T> operator*(const Matrix<rows, columns, T>& lhs, const Matrix<rows, columns, T>& rhs) {
	Matrix<rows, columns, T> result(0);
	for (size_t row = 0; row < rows; row++)
	{
		for (size_t column = 0; column < columns; column++)
		{
			for (size_t i = 0; i < rows; i++)
			{
				result(row, column) += lhs(row, i) * rhs(i, column);
			}
		}
	}
	return result;
}

template<typename T, size_t rows, size_t columns>
constexpr Matrix<rows, columns, T>& operator*=(Matrix<rows, columns, T>& lhs, const Matrix<rows, columns, T>& rhs) {
	lhs = lhs * rhs;
	return lhs;
}

template<typename T, size_t rows, size_t columns>
std::ostream& operator<<(std::ostream& stream, const Matrix<rows, columns, T>& matrix) {
	for (size_t row = 0; row < rows; row++)
	{
		stream << "(";
		for (size_t column = 0; column < columns; column++)
		{
			stream << (column == 0 ? "" : ", ") << matrix(row, column);
		}
		stream << ")" << std::endl;
	}
	return stream;
}

typedef Matrix<2, 2, int> Matrix2i;
typedef Matrix<2, 2, float> Matrix2;
typedef Matrix<2, 2, float> Matrix2f;
typedef Matrix<2, 2, double> Matrix2d;

typedef Matrix<3, 3, int> Matrix3i;
typedef Matrix<3, 3, float> Matrix3;
typedef Matrix<3, 3, float> Matrix3f;
typedef Matrix<3, 3, double> Matrix3d;

typedef Matrix<4, 4, int> Matrix4i;
typedef Matrix<4, 4, float> Matrix4;
typedef Matrix<4, 4, float> Matrix4f;
typedef Matrix<4, 4, double> Matrix4d;
