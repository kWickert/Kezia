#include "KeziaTrig.h"

#include <math.h>

namespace Kezia
{
	/*
	const U16 SinTableSize = 65535;
	const U16 CosOffset = SinTableSize >> 2;
	FixedPoint SinTable[65535];

	bool PopulateSinTable()
	{
		F32 inverseTableSize = 1.0f / SinTableSize;

		for(U32 i = 0; i < SinTableSize; i++)
		{
			SinTable[i] = sin(i * inverseTableSize * PI * 2.0f);
		}

		return true;
	}

	FixedPoint SinLookup(const KeziaAngle angle)
	{
		static bool tableInitialized = PopulateSinTable();
		return SinTable[angle];
	}

	FixedPoint Sin(const KeziaAngle angle)
	{
		return SinLookup(angle);
	}

	FixedPoint Cos(const KeziaAngle angle)
	{
		return SinLookup(angle + CosOffset);
	}
	*/
}