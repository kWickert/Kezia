#pragma once

#ifdef _DEBUG

#ifdef _WIN32
#include <crtdbg.h>
#endif

#endif

namespace Kezia
{
	inline void KeziaAssert(bool expr)
	{
#ifdef _DEBUG
#ifdef _WIN32
		_ASSERT(expr);
#endif
#endif
	}
}

