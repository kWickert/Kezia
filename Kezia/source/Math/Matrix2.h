#pragma once

#include "Vector2.h"

namespace Kezia
{
	template<typename T>
	class Matrix2
	{
	public:
		Matrix2(T m0 = 1, T m1 = 0, 
				T m2 = 0, T m3 = 1)
		{
			m_Data[0] = m0;
			m_Data[1] = m1;
			m_Data[2] = m2;
			m_Data[3] = m3;
		}

		Matrix2(const Matrix2 & other)
		{
			*this = other;
			return *this;
		}

		~Matrix2()
		{}

		Matrix2 & operator=(const Matrix2 & other)
		{
			for(size_t i = 0; i < 4; i++)
			{
				m_Data[i] = other.m_Data[i];
			}
		}

		T & operator[](const int index)
		{
			return m_Data[index];
		}

		const T & operator[](const int index) const
		{
			return m_Data[index];
		}

		Vector2<T> operator*(const Vector2<T> & rhs)
		{
			return Vector2<T>(	m_Data[0] * rhs[0] + m_Data[2] * rhs[1],
								m_Data[1] * rhs[0] + m_Data[3] * rhs[1]);
		}

		static Matrix2<T> Rotate(const T angleRadians = 0);
	private:
		T m_Data[4];
	};

	template<typename T>
	Matrix2<T> Matrix2<T>::Rotate(const T angleRadians)
	{
		T cos = std::cos(angleRadians);
		T sin = std::sin(angleRadians);

		return Matrix2<T>(	cos,	sin,
							-sin,	cos);
	}
}