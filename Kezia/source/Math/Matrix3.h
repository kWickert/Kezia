#pragma once

#include "Core/Types.h"

#include "Vector3.h"

namespace Kezia
{
	template<class T>
	class Matrix3
	{
	public:
		explicit Matrix3(	T v0 = 1,	T v1 = 0,	T v2 = 0,	
							T v3 = 0,	T v4 = 1,	T v5 = 0,	
							T v6 = 0,	T v7 = 0,	T v8 = 1)
		{
			m_Data[0] = v0;
			m_Data[1] = v1;
			m_Data[2] = v2;
			m_Data[3] = v3;
			m_Data[4] = v4;
			m_Data[5] = v5;
			m_Data[6] = v6;
			m_Data[7] = v7;
			m_Data[8] = v8;
		}

		explicit Matrix3(	const Vector3<T> & i,
							const Vector3<T> & j,
							const Vector3<T> & k)
		{
			m_Data[0] =	i.x();
			m_Data[1] =	i.y();
			m_Data[2] =	i.z();
			m_Data[3] = j.x();
			m_Data[4] = j.y();
			m_Data[5] = j.z();
			m_Data[6] = k.x();
			m_Data[7] = k.y();
			m_Data[8] = k.z();
		}

		explicit Matrix3(const T * data)
		{
			for(size_t i = 0; i < 9; ++i)
				m_Data[i] = data[i];
		}

		Matrix3(const Matrix3 & other)
		{
			for(size_t i = 0; i < 9; i++)
				m_Data[i] = other[i];
		}

		~Matrix3() {}

		Matrix3 & operator=(const Matrix3 & other)
		{
			Matrix3 tmp(other);
			swap(*this, tmp);
			return *this;
		}

		operator const T *() const
		{
			return m_Data;
		}

		operator char *()
		{
			return reinterpret_cast<char *>(m_Data);
		}

		operator const char *() const
		{
			return reinterpret_cast<const char *>(m_Data);
		}

		T & operator()(const U32 row, const U32 column)
		{
			return m_Data[row * 3 + column];
		}

		const T operator()(const U32 row, const U32 column) const
		{
			return m_Data[row * 3 + column];
		}

		T & operator[](const int index)
		{
			return m_Data[index];
		}

		const T operator[](const int index) const
		{
			return m_Data[index];
		}

		bool operator==(const Matrix3 & rhs)
		{
			for(size_t i = 0; i < 9; ++i)
			{
				if(m_Data[i] != rhs[i])
					return false;
			}

			return true;
		}

		bool operator!=(const Matrix3 & rhs)
		{
			return !(*this == rhs);
		}

		Matrix3 & operator*=(const Matrix3 & rhs)
		{
			Matrix3 tmp;

			tmp[0] =	m_Data[0] * rhs[0]	+ m_Data[3] * rhs[1]		+ m_Data[6] * rhs[2];
			tmp[1] =	m_Data[1] * rhs[0]	+ m_Data[4] * rhs[1]		+ m_Data[7] * rhs[2];
			tmp[2] =	m_Data[2] * rhs[0]	+ m_Data[5] * rhs[1]		+ m_Data[8] * rhs[2];

			tmp[3] =	m_Data[0] * rhs[3]	+ m_Data[3] * rhs[4]		+ m_Data[6] * rhs[5];
			tmp[4] =	m_Data[1] * rhs[3]	+ m_Data[4] * rhs[4]		+ m_Data[7] * rhs[5];
			tmp[5] =	m_Data[2] * rhs[3]	+ m_Data[5] * rhs[4]		+ m_Data[8] * rhs[5];

			tmp[6] =	m_Data[0] * rhs[6]	+ m_Data[3] * rhs[7]		+ m_Data[6] * rhs[8];
			tmp[7] =	m_Data[1] * rhs[6]	+ m_Data[4] * rhs[7]		+ m_Data[7] * rhs[8];
			tmp[8] =	m_Data[2] * rhs[6]	+ m_Data[5] * rhs[7]		+ m_Data[8] * rhs[8];

			swap(*this, tmp);

			return *this;
		}

		Matrix3 & operator+=(const Matrix3 & rhs)
		{
			for(size_t i = 0; i < 9; ++i)
				m_Data[i] += rhs[i];

			return *this;
		}

		Matrix3 & operator*=(const T scalar)
		{
			for(size_t i = 0; i < 9; ++i)
				m_Data[i] *= scalar;

			return *this;
		}

		Matrix3 & operator/=(const T scalar)
		{
			for(size_t i = 0; i < 9; ++i)
				m_Data[i] /= scalar;

			return *this;
		}

		Vector3<T> operator*(const Vector3<T> & rhs) const
		{
			return Vector3<T>(	m_Data[0] * rhs[0] + m_Data[3] * rhs[1] + m_Data[6] * rhs[2],
								m_Data[1] * rhs[0] + m_Data[4] * rhs[1] + m_Data[7] * rhs[2],
								m_Data[2] * rhs[0] + m_Data[5] * rhs[1] + m_Data[8] * rhs[2]);
		}

		void Transpose()
		{
			//in-place transpose
			std::swap(m_Data[1], m_Data[3]);
			std::swap(m_Data[2], m_Data[6]);
			std::swap(m_Data[5], m_Data[7]);
		}

		template<typename T>
		friend void swap(Matrix3<T> &, Matrix3<T> &);

		static Matrix3<T> Scale(const T x = 1, const T y = 1, const T z = 1);
					 
		static Matrix3<T> RotateX(const T angleRadians = 0);
		static Matrix3<T> RotateY(const T angleRadians = 0);
		static Matrix3<T> RotateZ(const T angleRadians = 0);
		static Matrix3<T> Rotate(const T angleRadians, Vector3<T> axis);

		static Matrix3<T> Translate(const T x = 0, const T y = 0);

		static Matrix3<T> LookAt(const Vector3<T> & camera, const Vector3<T> & target, const Vector3<T> & up = Vector3<T>(0, 1, 0));
	private:	
		T m_Data[9];
	};

	template<typename T>
	Matrix3<T> operator*(Matrix3<T> a, const Matrix3<T> & b)
	{
		a *= b;
		return a;
	}

	template<typename T>
	void swap(Matrix3<T> & a, Matrix3<T> & b)
	{
		for(size_t i = 0; i < 9; ++i)
			std::swap(a.m_Data[i], b.m_Data[i]);
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::Scale(const T x, const T y, const T z)
	{
		return Matrix3<T>(	x,	0,	0,
							0,	y,	0,
							0,	0,	z);
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::RotateX(const T angleRadians)
	{
		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		return Matrix3<T>(	1,	0,		0,
							0,	cos,	sin,
							0,	-sin,	cos);
	}

		template<typename T>
	Matrix3<T> Matrix3<T>::RotateY(const T angleRadians)
	{
		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		return Matrix3<T>(	cos,	0,	-sin,
							0,		1,	0,
							sin,	0,	cos);
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::RotateZ(const T angleRadians)
	{
		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		return Matrix3<T>(	cos,	sin,	0,
							-sin,	cos,	0,
							0,		0,		1);
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::Rotate(const T angleRadians, Vector3<T> axis)
	{
		if(axis.GetLengthSquared() != 1)
			axis = Normalize(axis);

		T xx = axis.x() * axis.x();
		T yy = axis.y() * axis.y();
		T zz = axis.z() * axis.z();

		T xy = axis.x() * axis.y();
		T xz = axis.x() * axis.z();
		T yz = axis.y() * axis.z();

		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		T t = 1 - cos;

		return Matrix3<T>(	xx * t + cos,					xy * t + axis.z() * sin,		xz * t - axis.y() * sin,
							xy * t - axis.z() * sin,		yy * t + cos,					yz * t + axis.x() * sin,
							xz * t + axis.y() * sin,		yz * t - axis.x() * sin,		zz * t + cos);
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::Translate(const T x, const T y)
	{
		return Matrix3<T>(	1,	0,	0,
							0,	1,	0,
							x,	y,	1);
	}

	template<typename T>
	Matrix3<T> Matrix3<T>::LookAt(	const Vector3<T> & camera,
									const Vector3<T> & target,
									const Vector3<T> & up)
	{
		Vector3<T> k(camera - target);
		k = Normalize(k);

		Vector3<T> i(CrossProduct(up, k));
		i = Normalize(i);

		Vector3<T> j(CrossProduct(k, i));
		j = Normalize(j);

		Matrix3<T> rotateInverse = Matrix3<T>(i, j, k);

		rotateInverse.Transpose();

		return rotateInverse;
	}
}