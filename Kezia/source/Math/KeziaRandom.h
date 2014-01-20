#pragma once

#include <random>

#include <Math/Vector2.h>
#include <Math/Matrix2.h>

#include "Core/Types.h"

namespace Kezia
{
	template<typename T>
	T RandomInRange(T min, T max)
	{
		F32 t = rand() / static_cast<F32>(RAND_MAX);

		return static_cast<T>(min + (max - min) * t);
	}

	template<typename T, typename VectorT>
	VectorT RandomVector(T min, T max)
	{
		VectorT v;

		for(U32 i = 0; i < v.NumDimensions; i++)
		{
			v[i] = RandomInRange(min, max);
		}

		return v;
	}

	template<typename T>
	Vector2<T> RandomUnitVector()
	{
		return Matrix2<T>::Rotate(RandomInRange<T>(0, 2 * PI)) * Vector2<T>(1, 0);
	}
}