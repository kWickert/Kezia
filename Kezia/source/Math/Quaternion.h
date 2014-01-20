#pragma once

#include "Math/MathUtility.h"
#include "Math/Matrix3.h"

namespace Kezia
{
	template<typename Value_T>
	class Quaternion
	{
	public:
		Quaternion(const Value_T w = 1, const Value_T x = 0, const Value_T y = 0, const Value_T z = 0)
			:	w(w), x(x), y(y), z(z)
		{}

		~Quaternion()
		{}

		operator const Matrix3<Value_T>() const
		{
			/*
			return Matrix3<Value_T>(
				1 - 2 * Square(y) - 2 * Square(z),	2 * x * y - 2 * z * w,				2 * x * z + 2 * y * w,
				2 * x * y + 2 * z * w,				1 - 2 * Square(x) - 2 * Square(z),	2 * y * z - 2 * x * w,
				2 * x * z + 2 * y * w,				2 * y * z + 2 * x * w,				1 - 2 * Square(x) - 2 * Square(y));
			*/
			return Matrix3<Value_T>(
				1 - 2 * Square(y) - 2 * Square(z),	2 * x * y + 2 * z * w,				2 * x * z - 2 * y * w,
				2 * x * y - 2 * z * w,				1 - 2 * Square(x) - 2 * Square(z),	2 * y * z + 2 * x * w,
				2 * x * z + 2 * y * w,				2 * y * z - 2 * x * w,				1 - 2 * Square(x) - 2 * Square(y));
		}

		Value_T w;
		Value_T x;
		Value_T y;
		Value_T z;
	};
}