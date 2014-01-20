#pragma once

#include "Core/Types.h"

#undef max

#include <limits>

namespace Kezia
{
	typedef U16 KeziaAngle;

	const F32 PI = 3.14159265359f;
	const F32 DegreesToRadiansMultiplier = PI / 180.0f;
	const F32 RadiansToDegreesMultiplier = 180.0f / PI;

	template <typename Degrees_T>
	inline KeziaAngle DegreesToKeziaAngle(const Degrees_T degrees)
	{
		F32 t = Mod(degrees, static_cast<Degrees_T>(360)) / 360.0f;
		return static_cast<KeziaAngle>(t * std::numeric_limits<KeziaAngle>::max());
	}

	template <typename Radians_T>
	inline KeziaAngle RadiansToKeziaAngle(const Radians_T radians)
	{
		F32 t = Mod(degrees, 2 * PI) / (2 * PI);
		return static_cast<KeziaAngle>(t * std::numeric_limits<KeziaAngle>::max());
	}

	template <typename Degrees_T>
	inline Degrees_T DegreesToRadians(const Degrees_T degrees)
	{
		return degrees * DegreesToRadiansMultiplier;
	}

	template <typename Radians_T>
	inline Radians_T RadiansToDegrees(const Radians_T radians)
	{
		return radians * RadiansToDegreesMultiplier;
	}
}