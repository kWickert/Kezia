#pragma once

#include "Core/Types.h"

namespace Kezia
{
	class Job
	{
	public:
		Job(const F32 priority = 0.5f);

		const F32 GetPriority() const;

		virtual void Execute() = 0;
	protected:
		F32 m_Priority;
	};
}