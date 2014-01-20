#pragma once

#include "Core/Types.h"

#undef max

#include <iostream>
#include <limits>

namespace Kezia
{
	template<class T>
	class Vector4
	{
	public:
		explicit Vector4(T value = 0)
		{
			for(U8 i = 0; i < NumDimensions; ++i)
				values[i] = value;
		}

		explicit Vector4(T x, T y, T z, T w)
		{
			values[0] = x;
			values[1] = y;
			values[2] = z;
			values[3] = w;
		}

		explicit Vector4(const T * v)
		{
			values[0] = v[0];
			values[1] = v[1];
			values[2] = v[2];
			values[3] = v[3];
		}

		Vector4(const Vector4 & other)
		{
			for(U8 i = 0; i < NumDimensions; ++i)
				values[i] = other.values[i];
		}

		~Vector4() 
		{}

		Vector4 & operator=(const Vector4 & other)
		{
			Vector4 tmp(other);
			swap(*this, tmp);
			return *this;
		}

		//cast operator to a vector containing a different type
		template<typename OtherT>
		operator Vector4<OtherT>()
		{
			return Vector4<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y), static_cast<OtherT>(z), static_cast<OtherT>(w));
		}

		bool operator==(const Vector4 & other) const
		{
			for(int i = 0; i < NumDimensions; ++i)
				if(operator[](i) != other[i])
					return false;

			return true;
		}

		bool operator!=(const Vector4 & other) const
		{
			return !operator==(other);
		}

		Vector4 & operator+=(const Vector4 & rhs)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) += rhs[i];

			return *this;
		}

		Vector4 & operator-=(const Vector4 & rhs)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) -= rhs[i];

			return *this;
		}

		Vector4 & operator*=(T scalar)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) *= scalar;

			return *this;
		}

		Vector4 & operator/=(T scalar)
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

		T & w()				{ return values[3]; }
		const T & w() const	{ return values[3]; }

		static const U8 NumDimensions = 4;

		T values[NumDimensions];
	};

	template<typename T>
	void swap(Vector4<T> & a, Vector4<T> & b)
	{
		for(U8 i = 0; i < a.NumDimensions; i++)
			std::swap(a[i], b[i]);
	}

	//note that lhs argument is always passed by value and
	//copied, therefore we can use op=(+=, -=, *=, /=) 
	//code reuse ftw

	//addition
	template<typename T>
	Vector4<T> operator+(Vector4<T> lhs, const Vector4<T> & rhs) { return lhs += rhs; }

	//subtraction
	template<typename T>
	Vector4<T> operator-(Vector4<T> lhs, const Vector4<T> & rhs) { return lhs -= rhs; }

	//multiplication : supports vector * scalar and scalar * vector
	template<typename T>
	Vector4<T> operator*(Vector4<T> vector, T scalar) { return vector *= scalar; }

	template<typename T>
	Vector4<T> operator*(T scalar, Vector4<T> vector) { return vector *= scalar; }

	//division
	template<typename T>
	Vector4<T> operator/(Vector4<T> vector, T scalar) { return vector /= scalar; }

	//unary negation
	template<typename T>
	Vector4<T> operator-(Vector4<T> vector) { return vector * static_cast<T>(-1); }

	//vector math
	template<typename T>
	T DotProduct(const Vector4<T> & lhs, const Vector4<T> & rhs)
	{
		T dot = 0;
		for(U8 i = 0; i < lhs.NumDimensions; ++i)
			dot += lhs[i] * rhs[i];

		return dot;
	}

	template<typename T>
	Vector4<T> Normalize(Vector4<T> vector)
	{
		T length = vector.GetLength();

		return length == 0 ? vector : (vector / length);
	}

	template<typename T>
	Vector4<T> Project(Vector4<T> source, const Vector4<T> & target)
	{
		return DotProduct(source, target) * target;
	}

	//stream operations
	template<typename T>
	std::ostream & operator<<(std::ostream & out, const Vector4<T> & v)
	{
		out << v[0];

		for(int i = 1; i < v.NumDimensions; i++)
		{
			out << ", " << v[i];
		}

		return out;
	}

	template<typename T>
	std::istream & operator>>(std::istream & in, Vector4<T> & v)
	{
		for(int i = 0; i < v.NumDimensions; i++)
		{
			in >> v[i];
			in.ignore(std::numeric_limits<std::streamsize>::max(), ',');
		}

		return in;
	}
}