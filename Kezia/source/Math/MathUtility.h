#pragma once

#include "Core/Types.h"

#include "Math/Vector2.h"

#include <math.h>
#include <stdlib.h>

namespace Kezia
{
	template<typename T>
	inline bool IsInRange(const T test, const T a, const T b)
	{
		bool aSmaller = a < b;
		T min, max;

		if(aSmaller)
		{
			min = a;
			max = b;
		}
		else
		{
			min = b;
			max = a;
		}

		return test >= min && test <= max;
	}

	template<typename T>
	inline T Square(T value)
	{
		return value * value;
	}

	template<typename T>
	inline T Cube(T value)
	{
		return value * value * value;
	}

	template<typename T>
	inline T AbsoluteValue(T value);

	template <>
	inline S32 AbsoluteValue(S32 value)
	{
		return abs(value);
	}

	template <>
	inline F32 AbsoluteValue(F32 value)
	{
		return fabs(value);
	}

	template <>
	inline F64 AbsoluteValue(F64 value)
	{
		return fabs(value);
	}

	inline const U32 ComputeManhattanDistance(const Vector2<U32> & a, const Vector2<U32> & b)
	{
		U32 distance = 0;

		for(U32 i = 0; i < Vector2<U32>::NumDimensions; i++)
		{
			distance += AbsoluteValue(static_cast<S32>(a[i]) - static_cast<S32>(b[i]));
		}

		return distance;
	}

	template<typename T>
	inline T Clamp(const T & min, const T & max, const T & value)
	{
		return (value > max ? max : value) < min ? min : value;
	}

	template<typename T>
	inline T Wrap(const T & min, const T & max, const T & value)
	{
		throw std::exception("fix this");
		T range = AbsoluteValue(max - min);

		T mod = Mod(value, range);

		return mod + (mod < 0 ? max : min);
	}

	template<typename T>
	inline T Lerp(const T & begin, const T & end, const T & interpValue)
	{
		return Clamp(begin, end, interpValue * (end - begin) + begin);
	}

	template <typename Value_T>
	inline Value_T Mod(const Value_T x, const Value_T y)
	{
		return x % y;
	}

	template <>
	inline F32 Mod(const F32 x, const F32 y)
	{
		return fmod(x, y);
	}

	template <>
	inline F64 Mod(const F64 x, const F64 y)
	{
		return fmod(x, y);
	}

	template <typename Value_T>
	inline Value_T Max(const Value_T a, const Value_T b)
	{
		return a >= b ? a : b;
	}

	template <typename Value_T>
	inline Value_T Min(const Value_T a, const Value_T b)
	{
		return a <= b ? a : b;
	}
}