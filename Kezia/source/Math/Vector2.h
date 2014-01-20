#pragma once

#include "Core/Types.h"

#include <iostream>

#undef max 

namespace Kezia
{
	template<typename T>
	class Vector2
	{
	public:
		Vector2(T value = 0)
		{
			for(U8 i = 0; i < NumDimensions; ++i)
				values[i] = value;
		}

		Vector2(T * data)
		{
			memcpy(values, data, sizeof(T) * 2);
		}

		Vector2(T x, T y)
		{
			values[0] = x;
			values[1] = y;
		}

		Vector2(const Vector2 & other)
		{
			for(U8 i = 0; i < NumDimensions; ++i)
				values[i] = other.values[i];
		}

		~Vector2()
		{}

		Vector2 & operator=(const Vector2 & other)
		{
			Vector2 tmp(other);
			swap(*this, tmp);
			return *this;
		}

		template<typename OtherT>
		operator Vector2<OtherT>()
		{
			Vector2<OtherT> tmp;

			for(U32 i = 0; i < NumDimensions; i++)
				tmp[i] = static_cast<OtherT>(values[i]);

			return tmp;
		}
			

		bool operator==(const Vector2 & other) const
		{
			for(int i = 0; i < NumDimensions; ++i)
				if(operator[](i) != other[i])
					return false;

			return true;
		}

		bool operator!=(const Vector2 & other) const
		{
			return !operator==(other);
		}

		Vector2 & operator+=(const Vector2 & rhs)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) += rhs[i];
			
			return *this;
		}

		Vector2 & operator-=(const Vector2 & rhs)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) -= rhs[i];

			return *this;
		}

		Vector2 & operator*=(T scalar)
		{
			for(int i = 0; i < NumDimensions; ++i)
				operator[](i) *= scalar;

			return *this;
		}

		Vector2 & operator/=(T scalar)
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

		static const U8 NumDimensions = 2;

		T values[NumDimensions];
	};

	template<typename T>
	void swap(Vector2<T> & a, Vector2<T> & b)
	{
		for(U8 i = 0; i < a.NumDimensions; i++)
			std::swap(a[i], b[i]);
	}

	//note that lhs argument is always passed by value and
	//copied, therefore we can use op=(+=, -=, *=, /=) 
	//code reuse ftw

	//addition
	template<typename T>
	Vector2<T> operator+(Vector2<T> lhs, const Vector2<T> & rhs) { return lhs += rhs; }

	//subtraction
	template<typename T>
	Vector2<T> operator-(Vector2<T> lhs, const Vector2<T> & rhs) { return lhs -= rhs; }

	//multiplication : supports vector * scalar and scalar * vector
	template<typename T>
	Vector2<T> operator*(Vector2<T> vector, T scalar) { return vector *= scalar; }

	template<typename T>
	Vector2<T> operator*(T scalar, Vector2<T> vector) { return vector *= scalar; }

	//division
	template<typename T>
	Vector2<T> operator/(Vector2<T> vector, T scalar) { return vector /= scalar; }

	//unary negation
	template<typename T>
	Vector2<T> operator-(Vector2<T> vector) { return vector * -1; }

	template<typename T>
	T DotProduct(const Vector2<T> & lhs, const Vector2<T> & rhs)
	{
		T dot = 0;
		for(U8 i = 0; i < lhs.NumDimensions; ++i)
			dot += lhs[i] * rhs[i];

		return dot;
	}

	template<typename T>
	Vector2<T> Normalize(Vector2<T> vector)
	{
		T length = vector.GetLength();

		return length == 0 ? vector : (vector / length);
	}

	template<typename T>
	Vector2<T> Project(Vector2<T> source, const Vector2<T> & target)
	{
		return DotProduct(source, target) * target;
	}

	template<typename T>
	void OrthoNormalize(Vector2<T> & v1, Vector2<T> & v2)
	{
		v2 = v2 - Project(v2, v1);

		v1 = Normalize(v1);
		v2 = Normalize(v2);
	}

	//stream operations
	template<typename T>
	std::ostream & operator<<(std::ostream & out, const Vector2<T> & v)
	{
		out << v[0];

		for(int i = 1; i < v.NumDimensions; i++)
		{
			out << ", " << v[i];
		}

		return out;
	}

	template<typename T>
	std::istream & operator>>(std::istream & in, Vector2<T> & v)
	{
		for(int i = 0; i < v.NumDimensions; i++)
		{
			in >> v[i];
			in.ignore(std::numeric_limits<std::streamsize>::max(), ',');
		}

		return in;
	}
}