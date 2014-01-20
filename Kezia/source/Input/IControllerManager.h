#pragma once

#include "IGameController.h"

namespace Kezia
{
	class IControllerManager
	{
	public:
		virtual IGameController * GetController(const U32 index) = 0;
		virtual const U32 GetActiveMotionControllerCount() const = 0;
	};
}