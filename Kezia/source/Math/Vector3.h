#pragma once

#include "Core/Types.h"

#undef max

#include <iostream>
#include <limits>

namespace Kezia
{
	template<class T>
	class Vector3
	{
	public:
		explicit Vector3(T value = 0)
		{
			for(U8 i = 0; i < NumDimensions; ++i)
				values[i] = value;
		}

		explicit Vector3(T x, T y, T z)
		{
			values[0] = x;
			values[1] = y;
			values[2] = z;
		}

		explicit Vector3(T * data)
		{
			memcpy(values, data, sizeof(T) * 3);
		}

		Vector3(const Vector3 & other)
		{
			for(U8 i = 0; i < NumDimensions; ++i)
				values[i] = other.values[i];
		}

		~Vector3() 
		{}

		Vector3 & operator=(const Vector3 & other)
		{
			Vector3 tmp(other);
			swap(*this, tmp);
			return *this;
		}

		//cast operator to a vector containing a different type
		template<typename OtherT>
		operator Vector3<OtherT>()
		{
			return Vector3<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y), static_cast<OtherT>(z));
		}

		operator char *()
		{
			return reinterpret_cast<char *>(values);
		}

		operator const char *() const
		{
			return reinterpret_cast<const char *>(values);
		}

		bool operator==(const Vector3 & other) const
		{
			for(int i = 0; i < NumDimensions; ++i)
				if(operator[](i) != other[i])
					return false;

			return true;
		}

		bool operator!=(const Vector3 & other) const
		{
			return !operator==(other);
		}

		Vector3 & operator+=(const Vector3 & rhs)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) += rhs[i];

			return *this;
		}

		Vector3 & operator-=(const Vector3 & rhs)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) -= rhs[i];

			return *this;
		}

		Vector3 & operator*=(T scalar)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) *= scalar;

			return *this;
		}

		Vector3 & operator/=(T scalar)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) /= scalar;

			return *this;
		}

		T GetLength() const
		{
			return std::sqrt(GetLengthSquared());
		}

		T GetLengthSquared() const
		{
			return DotProduct(*this, *this);
		}

		T & operator[](const int index) { return values[index]; }
		const T & operator[](const int index) const { return values[index]; }

		T & x()				{ return values[0]; }
		const T & x() const { return values[0]; }

		T & y()				{ return values[1]; }
		const T & y() const { return values[1]; }

		T & z()				{ return values[2]; }
		const T & z() const { return values[2]; }

		static const U8 NumDimensions = 3;

		T values[NumDimensions];
	};

	template<typename T>
	void swap(Vector3<T> & a, Vector3<T> & b)
	{
		for(U8 i = 0; i < a.NumDimensions; i++)
			std::swap(a[i], b[i]);
	}

	//note that lhs argument is always passed by value and
	//copied, therefore we can use op=(+=, -=, *=, /=) 
	//code reuse ftw

	//addition
	template<typename T>
	Vector3<T> operator+(Vector3<T> lhs, const Vector3<T> & rhs) { return lhs += rhs; }

	//subtraction
	template<typename T>
	Vector3<T> operator-(Vector3<T> lhs, const Vector3<T> & rhs) { return lhs -= rhs; }

	//multiplication : supports vector * scalar and scalar * vector
	template<typename T>
	Vector3<T> operator*(Vector3<T> vector, T scalar) { return vector *= scalar; }

	template<typename T>
	Vector3<T> operator*(T scalar, Vector3<T> vector) { return vector *= scalar; }

	//division
	template<typename T>
	Vector3<T> operator/(Vector3<T> vector, T scalar) { return vector /= scalar; }

	//unary negation
	template<typename T>
	Vector3<T> operator-(Vector3<T> vector) { return vector * static_cast<T>(-1); }

	//vector math
	template<typename T>
	T DotProduct(const Vector3<T> & lhs, const Vector3<T> & rhs)
	{
		T dot = 0;
		for(U8 i = 0; i < lhs.NumDimensions; ++i)
			dot += lhs[i] * rhs[i];

		return dot;
	}

	template<typename T>
	Vector3<T> CrossProduct(const Vector3<T> & lhs, const Vector3<T> & rhs)
	{
		return Vector3<T>(	lhs.y() * rhs.z() - rhs.y() * lhs.z(), 
							rhs.x() * lhs.z() - lhs.x() * rhs.z(),
							lhs.x() * rhs.y() - rhs.x() * lhs.y());
	}

	template<typename T>
	Vector3<T> Normalize(Vector3<T> vector)
	{
		T length = vector.GetLength();

		return length == 0 ? vector : (vector / length);
	}

	template<typename T>
	Vector3<T> Project(Vector3<T> source, const Vector3<T> & target)
	{
		return DotProduct(source, target) * target;
	}

	template<typename T>
	void OrthoNormalize(Vector3<T> & v1, Vector3<T> & v2, Vector3<T> & v3)
	{
		v2 = v2 - Project(v2, v1);
		v3 = v3 - Project(v3, v2) - Project(v3, v1);

		v1 = Normalize(v1);
		v2 = Normalize(v2);
		v3 = Normalize(v3);
	}

	//stream operations
	template<typename T>
	std::ostream & operator<<(std::ostream & out, const Vector3<T> & v)
	{
		out << v[0];

		for(int i = 1; i < v.NumDimensions; i++)
		{
			out << ", " << v[i];
		}

		return out;
	}

	template<typename T>
	std::istream & operator>>(std::istream & in, Vector3<T> & v)
	{
		for(int i = 0; i < v.NumDimensions; i++)
		{
			in >> v[i];
			in.ignore(std::numeric_limits<std::streamsize>::max(), ',');
		}

		return in;
	}
}