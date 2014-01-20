#pragma once

#include "Math/Matrix3.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include <algorithm>
#include <cmath>

namespace Kezia
{
	//4x4 column major matrix class
	//
	//	i	j	k	t		
	//	0	4	8	12		
	//	1	5	9	13		
	//	2	6	10	14		
	//	3	7	11	15		
	//
	//earlier transforms are pushed to
	//the right in multiplication chains
	//eg. m3 m2 m1 v

	template<class T>
	class Matrix4
	{
	public:
		explicit Matrix4(	T v0 = 1,	T v1 = 0,	T v2 = 0,	T v3 = 0,
							T v4 = 0,	T v5 = 1,	T v6 = 0,	T v7 = 0,
							T v8 = 0,	T v9 = 0,	T v10 = 1,	T v11 = 0,
							T v12 = 0,	T v13 = 0,	T v14 = 0,	T v15 = 1)
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
			m_Data[9] = v9;
			m_Data[10] = v10;
			m_Data[11] = v11;
			m_Data[12] = v12;
			m_Data[13] = v13;
			m_Data[14] = v14;
			m_Data[15] = v15;
		}

		explicit Matrix4(T * data)
		{
			for(size_t i = 0; i < 16; ++i)
				m_Data[i] = data[i];
		}
	
		Matrix4(const Matrix4 & other)
		{
			for(size_t i = 0; i < 16; i++)
				m_Data[i] = other[i];
		}

		Matrix4(const Matrix3<T> & orientation)
		{
			for(size_t i = 0; i < 16; i++)
				m_Data[i] = 0;

			S32 offset = -1;
			for(size_t i = 0; i < 9; i++)
			{
				if(i % 3 == 0)
					offset++;

				m_Data[i + offset] = orientation[i];				
			}

			m_Data[15] = 1;
		}

		Matrix4(const Quaternion<T> & orientation)
		{
			for(size_t i = 0; i < 16; i++)
				m_Data[i] = 0;

			T & x = orientation.x;
			T & y = orientation.y;
			T & z = orientation.z;
			T & w = orientation.w;

			m_Data[0] = 1 - 2 * Square(y) - 2 * Square(z);
			m_Data[1] = 2 * x * y + 2 * z * w;
			m_Data[2] = 2 * x * z - 2 * y * w;
			m_Data[3] = 0;
			m_Data[4] = 2 * x * y - 2 * z * w;
			m_Data[5] = 1 - 2 * Square(x) - 2 * Square(z);
			m_Data[6] = 2 * y * z + 2 * x * w;
			m_Data[7] = 0;
			m_Data[8] = 2 * x * z + 2 * y * w;
			m_Data[9] = 2 * y * z - 2 * x * w;
			m_Data[10] = 1 - 2 * Square(x) - 2 * Square(y);
			m_Data[11] = 0;
			m_Data[12] = 0;
			m_Data[13] = 0;
			m_Data[14] = 0;
			m_Data[15] = 1;
		}

		~Matrix4() {}

		Matrix4 & operator=(const Matrix4 & other)
		{
			Matrix4 tmp(other);
			swap(*this, tmp);
			return *this;
		}

		operator const T *() const
		{
			return m_Data;
		}

		T & operator[](const int index)
		{
			return m_Data[index];
		}

		const T operator[](const int index) const
		{
			return m_Data[index];
		}

		bool operator==(const Matrix4 & rhs)
		{
			for(size_t i = 0; i < 16; ++i)
			{
				if(m_Data[i] != rhs[i])
					return false;
			}

			return true;
		}

		bool operator!=(const Matrix4 & rhs)
		{
			return !(*this == rhs);
		}

		Matrix4 & operator+=(const Matrix4 & rhs)
		{
			for(size_t i = 0; i < 16; ++i)
				m_Data[i] += rhs[i];

			return *this;
		}

		Matrix4 & operator-=(const Matrix4 & rhs)
		{
			for(size_t i = 0; i < 16; ++i)
				m_Data[i] /= rhs[i];

			return *this;
		}

		Matrix4 & operator*=(const Matrix4 & rhs)
		{
			Matrix4 tmp;

			tmp[0] =	m_Data[0] * rhs[0]	+ m_Data[4] * rhs[1]		+ m_Data[8] * rhs[2]		+ m_Data[12] * rhs[3];
			tmp[1] =	m_Data[1] * rhs[0]	+ m_Data[5] * rhs[1]		+ m_Data[9] * rhs[2]		+ m_Data[13] * rhs[3];
			tmp[2] =	m_Data[2] * rhs[0]	+ m_Data[6] * rhs[1]		+ m_Data[10] * rhs[2]		+ m_Data[14] * rhs[3];
			tmp[3] =	m_Data[3] * rhs[0]	+ m_Data[7] * rhs[1]		+ m_Data[11] * rhs[2]		+ m_Data[15] * rhs[3];

			tmp[4] =	m_Data[0] * rhs[4]	+ m_Data[4] * rhs[5]		+ m_Data[8] * rhs[6]		+ m_Data[12] * rhs[7];
			tmp[5] =	m_Data[1] * rhs[4]	+ m_Data[5] * rhs[5]		+ m_Data[9] * rhs[6]		+ m_Data[13] * rhs[7];
			tmp[6] =	m_Data[2] * rhs[4]	+ m_Data[6] * rhs[5]		+ m_Data[10] * rhs[6]		+ m_Data[14] * rhs[7];
			tmp[7] =	m_Data[3] * rhs[4]	+ m_Data[7] * rhs[5]		+ m_Data[11] * rhs[6]		+ m_Data[15] * rhs[7];

			tmp[8] =	m_Data[0] * rhs[8]	+ m_Data[4] * rhs[9]		+ m_Data[8] * rhs[10]		+ m_Data[12] * rhs[11];
			tmp[9] =	m_Data[1] * rhs[8]	+ m_Data[5] * rhs[9]		+ m_Data[9] * rhs[10]		+ m_Data[13] * rhs[11];
			tmp[10] =	m_Data[2] * rhs[8]	+ m_Data[6] * rhs[9]		+ m_Data[10] * rhs[10]		+ m_Data[14] * rhs[11];
			tmp[11] =	m_Data[3] * rhs[8]	+ m_Data[7] * rhs[9]		+ m_Data[11] * rhs[10]		+ m_Data[15] * rhs[11];

			tmp[12] =	m_Data[0] * rhs[12]	+ m_Data[4] * rhs[13]	+ m_Data[8] * rhs[14]			+ m_Data[12] * rhs[15];
			tmp[13] =	m_Data[1] * rhs[12]	+ m_Data[5] * rhs[13]	+ m_Data[9] * rhs[14]			+ m_Data[13] * rhs[15];	
			tmp[14] =	m_Data[2] * rhs[12]	+ m_Data[6] * rhs[13]	+ m_Data[10] * rhs[14]			+ m_Data[14] * rhs[15];	
			tmp[15] =	m_Data[3] * rhs[12]	+ m_Data[7] * rhs[13]	+ m_Data[11] * rhs[14]			+ m_Data[15] * rhs[15];

			swap(*this, tmp);

			return *this;
		}

		Matrix4 & operator/=(const T scalar)
		{
			for(size_t i = 0; i < 16; ++i)
				m_Data[i] /= scalar;

			return *this;
		}

		Vector4<T> operator*(const Vector4<T> & rhs)
		{
			return Vector4<T>(	m_Data[0] * rhs[0] + m_Data[4] * rhs[1] + m_Data[8] * rhs[2] + m_Data[12] * rhs[3],
								m_Data[1] * rhs[0] + m_Data[5] * rhs[1] + m_Data[9] * rhs[2] + m_Data[13] * rhs[3],
								m_Data[2] * rhs[0] + m_Data[6] * rhs[1] + m_Data[10] * rhs[2] + m_Data[14] * rhs[3],
								m_Data[3] * rhs[0] + m_Data[7] * rhs[1] + m_Data[11] * rhs[2] + m_Data[15] * rhs[3]);
		}

		void Transpose()
		{
			//in-place transpose
			std::swap(m_Data[1], m_Data[4]);
			std::swap(m_Data[2], m_Data[8]);
			std::swap(m_Data[3], m_Data[12]);
			std::swap(m_Data[6], m_Data[9]);
			std::swap(m_Data[7], m_Data[13]);
			std::swap(m_Data[11],m_Data[14]);
		}

		template<typename T>
		friend void swap(Matrix4<T> &, Matrix4<T> &);

		static Matrix4<T> Scale(const T x = 1, const T y = 1, const T z = 1);

		static Matrix4<T> RotateX(const T angleRadians = 0);
		static Matrix4<T> RotateY(const T angleRadians = 0);
		static Matrix4<T> RotateZ(const T angleRadians = 0);
		static Matrix4<T> Rotate(const T angleRadians, Vector3<T> axis);

		static Matrix4<T> Translate(const T x = 0, const T y = 0, const T z = 0);
		static Matrix4<T> Translate(const Vector3<F32> & position = Vector3<F32>());

		static Matrix4<T> LookAt(const Vector3<T> & camera, const Vector3<T> & target, const Vector3<T> & up = Vector3<T>(0, 1, 0));

		static Matrix4<T> Orthographic(T left, T right, T bottom, T top, T nearPlane = -1, T farPlane = 1);
		static Matrix4<T> Perspective(T fieldOfViewY, T aspectRatio, T nearPlane, T farPlane);
	private:	
		T m_Data[16];
	};

	template<typename T>
	Matrix4<T> operator*(Matrix4<T> a, const Matrix4<T> & b)
	{
		a *= b;
		return a;
	}

	template<typename T>
	void swap(Matrix4<T> & a, Matrix4<T> & b)
	{
		for(size_t i = 0; i < 16; ++i)
			std::swap(a.m_Data[i], b.m_Data[i]);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::Scale(const T x, const T y, const T z)
	{
		return Matrix4<T>(	x,	0,	0,	0,
							0,	y,	0,	0,
							0,	0,	z,	0,
							0,	0,	0,	1);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::RotateX(const T angleRadians)
	{
		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		return Matrix4<T>(	1,	0,		0,		0,
							0,	cos,	sin,	0,
							0,	-sin,	cos,	0,
							0,	0,		0,		1);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::RotateY(const T angleRadians)
	{
		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		return Matrix4<T>(	cos,	0,	-sin,	0,
							0,		1,	0,		0,
							sin,	0,	cos,	0,
							0,		0,	0,		1);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::RotateZ(const T angleRadians)
	{
		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		return Matrix4<T>(	cos,	sin,	0,	0,
							-sin,	cos,	0,	0,
							0,		0,		1,	0,
							0,		0,		0,	1);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::Rotate(const T angleRadians, Vector3<T> axis)
	{
		if(axis.GetLengthSquared() != 1)
			axis = Normalize(axis);

		T x = axis.x();
		T y = axis.y();
		T z = axis.z();

		T xx = x * x;
		T yy = y * y;
		T zz = z * z;
							  
		T xy = x * y;
		T xz = x * z;
		T yz = y * z;

		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		T t = 1 - cos;

		return Matrix4<T>(	xx * t + cos,				xy * t + z * sin,			xz * t - y * sin,	0,
							xy * t - z * sin,			yy * t + cos,				yz * t + x * sin,	0,
							xz * t + y * sin,			yz * t - x * sin,			zz * t + cos,		0,
							0,							0,							0,					1);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::Translate(const T x, const T y, const T z)
	{
		return Matrix4<T>(	1,	0,	0,	0,
							0,	1,	0,	0,
							0,	0,	1,	0,
							x,	y,	z,	1);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::Translate(const Vector3<F32> & position)
	{
		return Translate(position.x(), position.y(), position.z());
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::LookAt(	const Vector3<T> & camera,
									const Vector3<T> & target,
									const Vector3<T> & up)
	{
		Vector3<T> k(camera - target);
		k = Normalize(k);

		Vector3<T> i(CrossProduct(up, k));
		i = Normalize(i);

		Vector3<T> j(CrossProduct(k, i));
		j = Normalize(j);

		Matrix4<T> rotateInverse = Matrix4<T>(	i.x(),	i.y(),	i.z(),	0.0,
												j.x(),	j.y(),	j.z(),	0.0,
												k.x(),	k.y(),	k.z(),	0.0,
												0.0,	0.0,	0.0,	1.0);

		rotateInverse.Transpose();

		Matrix4<T> translateInverse = Translate(-camera.x(), -camera.y(), -camera.z());

		return rotateInverse * translateInverse;
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::Orthographic(T left, T right, T bottom, T top, T nearPlane, T farPlane)
	{
		T rlDiff = right - left;
		T tbDiff = top - bottom;
		T fnDiff = farPlane - nearPlane;

		T tX = -(right + left) / rlDiff;
		T tY = -(top + bottom) / tbDiff;
		T tZ = -(farPlane + nearPlane) / fnDiff;

		return Matrix4<T>(	2 / rlDiff,	0,			0,				0,
							0,			2 / tbDiff,	0,				0,
							0,			0,			-2 / fnDiff,	0,
							tX,			tY,			tZ,				1);
	}

	template<typename T>
	Matrix4<T> Matrix4<T>::Perspective(T fieldOfViewY, T aspectRatio, T nearPlane, T farPlane)
	{
		T angle = fieldOfViewY / 2;
		T f = 1 / tan(angle);

		T zDiff = nearPlane - farPlane;

		return Matrix4<T>(	f / aspectRatio,	0,	0,									0,
							0,					f,	0,									0,
							0,					0,	(farPlane + nearPlane) / zDiff,		-1,
							0,					0,	(2 * farPlane * nearPlane) / zDiff,	0);
	}

	template<typename T>
	inline Matrix3<T> ExtractOrientationComponent(const Matrix4<T> & mat)
	{
		return Matrix3<F32>(	mat[0], mat[1], mat[2],
								mat[4], mat[5], mat[6],
								mat[8], mat[9], mat[10]);	
	}

	template<typename T>
	inline Vector3<T> ExtractTranslationComponent(const Matrix4<T> & mat)
	{
		return Vector3<F32>(mat[12], mat[13], mat[14]);
	}
}