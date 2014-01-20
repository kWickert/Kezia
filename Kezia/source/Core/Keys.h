#pragma once

#include "Core/Types.h"

namespace Kezia
{
	const U8 SpecialKeyBegin = CHAR_MAX + 1;

	enum KeziaKeys
	{
		KZ_END = SpecialKeyBegin,
		KZ_HOME,
		KZ_LEFT,
		KZ_UP,
		KZ_RIGHT,
		KZ_DOWN,
		KZ_SELECT,
		KZ_PRINT,
		KZ_EXECUTE,
		KZ_SNAPSHOT,
		KZ_INSERT,
		KZ_DELETE,
		KZ_HELP
	};
}